// Parts 2-4 - estimateDeliveryDays, chooseBox, shippingCostCents

#include <stdexcept>

#include "TestFixtures.h"
#include "TestHarness.h"

using namespace fixtures;

namespace {

FulfillmentPlanner emptyPlanner() { return FulfillmentPlanner(makeCatalog(), {}); }

}  // namespace

TEST_CASE(T05_estimates_delivery_days_for_prime_and_standard) {
    FulfillmentPlanner planner = emptyPlanner();
    Order prime = primeOrder(1, {0, 0}, {{KINDLE, 1}});
    Order standard = standardOrder(2, {0, 0}, {{KINDLE, 1}});

    EXPECT_EQ(planner.estimateDeliveryDays(prime, 0), 1);
    EXPECT_EQ(planner.estimateDeliveryDays(prime, 50), 1);
    EXPECT_EQ(planner.estimateDeliveryDays(prime, 51), 2);
    EXPECT_EQ(planner.estimateDeliveryDays(prime, 200), 2);
    EXPECT_EQ(planner.estimateDeliveryDays(prime, 201), 3);

    EXPECT_EQ(planner.estimateDeliveryDays(standard, 0), 3);
    EXPECT_EQ(planner.estimateDeliveryDays(standard, 99), 3);
    EXPECT_EQ(planner.estimateDeliveryDays(standard, 100), 4);
    EXPECT_EQ(planner.estimateDeliveryDays(standard, 250), 5);
    EXPECT_EQ(planner.estimateDeliveryDays(standard, 699), 9);
    EXPECT_EQ(planner.estimateDeliveryDays(standard, 700), 10);
    EXPECT_EQ(planner.estimateDeliveryDays(standard, 5000), 10);

    EXPECT_THROWS(planner.estimateDeliveryDays(prime, -1), std::invalid_argument);
    EXPECT_THROWS(planner.estimateDeliveryDays(standard, -1), std::invalid_argument);
}

TEST_CASE(T06_chooses_box_by_volume_and_weight) {
    FulfillmentPlanner planner = emptyPlanner();
    auto box = [&](std::vector<OrderLine> lines) {
        return planner.chooseBox(standardOrder(1, {0, 0}, std::move(lines)));
    };

    EXPECT_EQ(box({{KINDLE, 2}}), BoxSize::Small);                      // 1400 cm3
    EXPECT_EQ(box({{KINDLE, 2}, {FIRE_TV, 2}}), BoxSize::Small);        // 2000 cm3
    EXPECT_EQ(box({{ECHO_DOT, 3}}), BoxSize::Medium);                   // 3000 cm3
    EXPECT_EQ(box({{SKILLET, 2}}), BoxSize::Medium);                    // 8000 cm3
    EXPECT_EQ(box({{SKILLET, 3}}), BoxSize::Large);                     // 12000 cm3
    EXPECT_EQ(box({{WATER_24, 3}}), BoxSize::Large);                    // 36000 cm3
    EXPECT_EQ(box({{WATER_24, 4}}), BoxSize::Freight);                  // 48000 cm3
    EXPECT_EQ(box({{DESKCHAIR, 1}}), BoxSize::Freight);                 // 90000 cm3

    // Heavy-but-small orders.
    EXPECT_EQ(box({{DUMBBELL, 1}}), BoxSize::Small);                    // 1500 cm3,  9000 g
    EXPECT_EQ(box({{DUMBBELL, 1}, {SKILLET, 2}}), BoxSize::Medium);     // 9500 cm3, 15000 g
    EXPECT_EQ(box({{DUMBBELL, 2}}), BoxSize::Large);                    // 3000 cm3, 18000 g
}

TEST_CASE(T07_computes_shipping_cost_in_cents) {
    FulfillmentPlanner planner = emptyPlanner();

    // Prime ships free...
    EXPECT_EQ(planner.shippingCostCents(primeOrder(1, {0, 0}, {{KINDLE, 1}}), 300), 0LL);

    // ...standard pays base + per-started-kg + per-distance.
    EXPECT_EQ(planner.shippingCostCents(standardOrder(2, {0, 0}, {{KINDLE, 1}}), 0), 598LL);
    EXPECT_EQ(planner.shippingCostCents(standardOrder(3, {0, 0}, {{SKILLET, 1}}), 40), 876LL);
    EXPECT_EQ(planner.shippingCostCents(standardOrder(4, {0, 0}, {{ECHO_DOT, 4}}), 15), 727LL);

    // Freight surcharge applies to everyone.
    EXPECT_EQ(planner.shippingCostCents(standardOrder(5, {0, 0}, {{DESKCHAIR, 1}}), 10), 4603LL);
    EXPECT_EQ(planner.shippingCostCents(primeOrder(6, {0, 0}, {{DESKCHAIR, 1}}), 10), 2500LL);

    EXPECT_THROWS(planner.shippingCostCents(standardOrder(7, {0, 0}, {{KINDLE, 1}}), -5),
                  std::invalid_argument);
}
