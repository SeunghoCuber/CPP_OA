#pragma once

#include <optional>
#include <vector>

#include "Catalog.h"
#include "FulfillmentCenter.h"
#include "Order.h"
#include "Shipment.h"

namespace fulfillment {

// Decides where, how, and at what cost customer orders are shipped.
//
// Full behavioral specification: docs/PROBLEM_STATEMENT.md
// Implementation lives in src/FulfillmentPlanner.cpp (the only file you edit).
class FulfillmentPlanner {
public:
    // Throws std::invalid_argument if two centers share the same id.
    FulfillmentPlanner(Catalog catalog, std::vector<FulfillmentCenter> centers);

    // Part 1 - picks the fulfillment center that should ship the order.
    std::optional<int> selectFulfillmentCenter(const Order& order) const;

    // Part 2 - promised delivery time in whole days.
    int estimateDeliveryDays(const Order& order, int distance) const;

    // Part 3 - the box the order ships in.
    BoxSize chooseBox(const Order& order) const;

    // Part 4 - what the customer pays for shipping, in cents.
    long long shippingCostCents(const Order& order, int distance) const;

    // Part 5 - plans a batch of orders, reserving inventory as it goes.
    BatchPlan planBatch(const std::vector<Order>& orders);

    // Read access to a center by id. Throws std::out_of_range if unknown.
    const FulfillmentCenter& center(int id) const;

    const Catalog& catalog() const { return catalog_; }
    const std::vector<FulfillmentCenter>& centers() const { return centers_; }

private:
    Catalog catalog_;
    std::vector<FulfillmentCenter> centers_;
};

}  // namespace fulfillment
