// Part 1 - selectFulfillmentCenter

#include "TestFixtures.h"
#include "TestHarness.h"

using namespace fixtures;

TEST_CASE(T01_selects_nearest_center_with_stock) {
    FulfillmentPlanner planner(makeCatalog(),
                               {makeCenter(1, "SEA1", {0, 0}, {{KINDLE, 5}}),
                                makeCenter(4, "BFI4", {30, 40}, {{KINDLE, 5}}),
                                makeCenter(9, "PDX9", {200, 0}, {{KINDLE, 5}})});

    // Distances: SEA1 = 60, BFI4 = 10, PDX9 = 210
    EXPECT_EQ(planner.selectFulfillmentCenter(standardOrder(1, {25, 35}, {{KINDLE, 1}})),
              std::optional<int>(4));

    // Distances: SEA1 = 195, BFI4 = 195, PDX9 = 15
    EXPECT_EQ(planner.selectFulfillmentCenter(primeOrder(2, {190, 5}, {{KINDLE, 1}})),
              std::optional<int>(9));
}

TEST_CASE(T02_breaks_distance_ties_by_lowest_center_id) {
    // Note: the lowest id is deliberately NOT first in the list.
    FulfillmentPlanner planner(makeCatalog(),
                               {makeCenter(7, "DFW7", {10, 0}, {{ECHO_DOT, 3}}),
                                makeCenter(3, "ORD3", {-10, 0}, {{ECHO_DOT, 3}}),
                                makeCenter(12, "JFK12", {0, 10}, {{ECHO_DOT, 3}}),
                                makeCenter(2, "MDW2", {0, 11}, {{ECHO_DOT, 3}})});

    // DFW7, ORD3 and JFK12 are all at distance 10; MDW2 is at distance 11.
    EXPECT_EQ(planner.selectFulfillmentCenter(standardOrder(1, {0, 0}, {{ECHO_DOT, 1}})),
              std::optional<int>(3));
}

TEST_CASE(T03_requires_full_stock_for_every_sku_in_the_order) {
    FulfillmentPlanner planner(
        makeCatalog(),
        {makeCenter(1, "SEA1", {0, 0}, {{ECHO_DOT, 2}, {KINDLE, 10}}),
         makeCenter(2, "PDX9", {50, 50}, {{ECHO_DOT, 5}, {KINDLE, 10}, {SKILLET, 1}})});

    // Echo Dot appears on two lines: 2 + 1 = 3 units needed, SEA1 only has 2.
    Order splitLines = standardOrder(1, {0, 0}, {{ECHO_DOT, 2}, {KINDLE, 1}, {ECHO_DOT, 1}});
    EXPECT_EQ(planner.selectFulfillmentCenter(splitLines), std::optional<int>(2));

    // SEA1 has never stocked skillets.
    Order withSkillet = standardOrder(2, {0, 0}, {{KINDLE, 1}, {SKILLET, 1}});
    EXPECT_EQ(planner.selectFulfillmentCenter(withSkillet), std::optional<int>(2));

    // Exactly the available quantity is enough.
    Order exactStock = standardOrder(3, {0, 0}, {{KINDLE, 10}});
    EXPECT_EQ(planner.selectFulfillmentCenter(exactStock), std::optional<int>(1));

    // Selection must never change inventory.
    EXPECT_EQ(planner.center(1).inventory().available(ECHO_DOT), 2);
    EXPECT_EQ(planner.center(1).inventory().available(KINDLE), 10);
}

TEST_CASE(T04_returns_nullopt_when_no_center_can_fulfill) {
    FulfillmentPlanner planner(makeCatalog(), {makeCenter(1, "SEA1", {0, 0}, {{KINDLE, 1}})});

    EXPECT_EQ(planner.selectFulfillmentCenter(standardOrder(1, {0, 0}, {{KINDLE, 2}})),
              std::nullopt);
    EXPECT_EQ(planner.selectFulfillmentCenter(standardOrder(2, {0, 0}, {{DESKCHAIR, 1}})),
              std::nullopt);

    FulfillmentPlanner noCenters(makeCatalog(), {});
    EXPECT_EQ(noCenters.selectFulfillmentCenter(primeOrder(3, {0, 0}, {{KINDLE, 1}})),
              std::nullopt);
}
