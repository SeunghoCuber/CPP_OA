// =============================================================================
//  FulfillmentPlanner.cpp
//
//  >>> THIS IS THE ONLY FILE YOU SHOULD MODIFY. <<<
//
//  Implement the five TODO methods below according to docs/PROBLEM_STATEMENT.md.
//  The constructor and center() are already implemented for you.
//
//  You may add #includes and helper functions / constants (e.g. in the
//  anonymous namespace below). You may NOT change any header, test, or other
//  source file - the graders build against the originals.
// =============================================================================

#include "FulfillmentPlanner.h"

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>

namespace fulfillment {

namespace {

// Add any helper functions or constants you need here.

}  // namespace

// -----------------------------------------------------------------------------
// Provided - do not change the behavior of these.
// -----------------------------------------------------------------------------

FulfillmentPlanner::FulfillmentPlanner(Catalog catalog, std::vector<FulfillmentCenter> centers)
    : catalog_(std::move(catalog)), centers_(std::move(centers)) {
    std::unordered_set<int> seen;
    for (const auto& fc : centers_) {
        if (!seen.insert(fc.id()).second) {
            throw std::invalid_argument("FulfillmentPlanner: duplicate center id " +
                                        std::to_string(fc.id()));
        }
    }
}

const FulfillmentCenter& FulfillmentPlanner::center(int id) const {
    for (const auto& fc : centers_) {
        if (fc.id() == id) {
            return fc;
        }
    }
    throw std::out_of_range("FulfillmentPlanner: unknown center id " + std::to_string(id));
}

// -----------------------------------------------------------------------------
// Part 1 - Fulfillment center selection        (docs/PROBLEM_STATEMENT.md §1)
// -----------------------------------------------------------------------------
std::optional<int> FulfillmentPlanner::selectFulfillmentCenter(const Order& order) const {
    // TODO: implement
    (void)order;
    return std::nullopt;
}

// -----------------------------------------------------------------------------
// Part 2 - Delivery promise                    (docs/PROBLEM_STATEMENT.md §2)
// -----------------------------------------------------------------------------
int FulfillmentPlanner::estimateDeliveryDays(const Order& order, int distance) const {
    // TODO: implement
    (void)order;
    (void)distance;
    return 0;
}

// -----------------------------------------------------------------------------
// Part 3 - Box selection                       (docs/PROBLEM_STATEMENT.md §3)
// -----------------------------------------------------------------------------
BoxSize FulfillmentPlanner::chooseBox(const Order& order) const {
    // TODO: implement
    (void)order;
    return BoxSize::Small;
}

// -----------------------------------------------------------------------------
// Part 4 - Shipping cost                       (docs/PROBLEM_STATEMENT.md §4)
// -----------------------------------------------------------------------------
long long FulfillmentPlanner::shippingCostCents(const Order& order, int distance) const {
    // TODO: implement
    (void)order;
    (void)distance;
    return 0;
}

// -----------------------------------------------------------------------------
// Part 5 - Batch planning                      (docs/PROBLEM_STATEMENT.md §5)
// -----------------------------------------------------------------------------
BatchPlan FulfillmentPlanner::planBatch(const std::vector<Order>& orders) {
    // TODO: implement
    (void)orders;
    return {};
}

}  // namespace fulfillment
