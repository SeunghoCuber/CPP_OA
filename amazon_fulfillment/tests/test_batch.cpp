// Part 5 - planBatch

#include <stdexcept>
#include <vector>

#include "TestFixtures.h"
#include "TestHarness.h"

using namespace fixtures;

namespace {

std::vector<int> shipmentOrderIds(const BatchPlan& plan) {
    std::vector<int> ids;
    for (const auto& s : plan.shipments) ids.push_back(s.orderId);
    return ids;
}

}  // namespace

TEST_CASE(T08_plans_batch_by_priority_and_reserves_inventory) {
    FulfillmentPlanner planner(makeCatalog(),
                               {makeCenter(1, "SEA1", {0, 0}, {{KINDLE, 1}, {ECHO_DOT, 1}}),
                                makeCenter(2, "PDX9", {100, 0}, {{KINDLE, 5}})});

    std::vector<Order> orders = {
        standardOrder(101, {0, 0}, {{KINDLE, 1}}, /*placedAt=*/1000),
        primeOrder(102, {0, 0}, {{ECHO_DOT, 1}}, /*placedAt=*/3000),
        primeOrder(103, {0, 5}, {{KINDLE, 1}, {ECHO_DOT, 1}}, /*placedAt=*/2000),
        standardOrder(104, {0, 0}, {{ECHO_DOT, 1}}, /*placedAt=*/1000),
        standardOrder(100, {90, 0}, {{KINDLE, 2}}, /*placedAt=*/1000),
    };

    BatchPlan plan = planner.planBatch(orders);

    EXPECT_EQ(shipmentOrderIds(plan), (std::vector<int>{103, 100, 101}));
    EXPECT_EQ(plan.backorderedOrderIds, (std::vector<int>{102, 104}));

    const Shipment& first = plan.shipments.at(0);
    EXPECT_EQ(first.fulfillmentCenterId, 1);
    EXPECT_EQ(first.distance, 5);
    EXPECT_EQ(first.deliveryDays, 1);
    EXPECT_EQ(first.box, BoxSize::Small);
    EXPECT_EQ(first.shippingCostCents, 0LL);

    const Shipment& second = plan.shipments.at(1);
    EXPECT_EQ(second.fulfillmentCenterId, 2);
    EXPECT_EQ(second.distance, 10);
    EXPECT_EQ(second.deliveryDays, 3);
    EXPECT_EQ(second.box, BoxSize::Small);
    EXPECT_EQ(second.shippingCostCents, 618LL);

    const Shipment& third = plan.shipments.at(2);
    EXPECT_EQ(third.fulfillmentCenterId, 2);
    EXPECT_EQ(third.distance, 100);
    EXPECT_EQ(third.deliveryDays, 4);
    EXPECT_EQ(third.shippingCostCents, 798LL);

    EXPECT_EQ(planner.center(1).inventory().available(KINDLE), 0);
    EXPECT_EQ(planner.center(1).inventory().available(ECHO_DOT), 0);
    EXPECT_EQ(planner.center(2).inventory().available(KINDLE), 2);
}

TEST_CASE(T09_empty_batch_produces_empty_plan) {
    FulfillmentPlanner planner(makeCatalog(), {makeCenter(1, "SEA1", {0, 0}, {{KINDLE, 3}})});

    BatchPlan plan = planner.planBatch({});

    EXPECT_TRUE(plan.shipments.empty());
    EXPECT_TRUE(plan.backorderedOrderIds.empty());
    EXPECT_EQ(planner.center(1).inventory().available(KINDLE), 3);
}

TEST_CASE(T10_rejects_duplicate_order_ids_without_touching_inventory) {
    FulfillmentPlanner planner(makeCatalog(), {makeCenter(1, "SEA1", {0, 0}, {{KINDLE, 5}})});

    std::vector<Order> orders = {
        standardOrder(7, {0, 0}, {{KINDLE, 1}}, 10),
        primeOrder(8, {0, 0}, {{KINDLE, 1}}, 20),
        standardOrder(7, {3, 3}, {{KINDLE, 2}}, 30),
    };

    EXPECT_THROWS(planner.planBatch(orders), std::invalid_argument);
    EXPECT_EQ(planner.center(1).inventory().available(KINDLE), 5);
}
