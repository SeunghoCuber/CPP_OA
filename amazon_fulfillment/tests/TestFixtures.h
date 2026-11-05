#pragma once

// Shared test data: a small Amazon catalog and helpers for building
// fulfillment centers and orders.

#include <string>
#include <utility>
#include <vector>

#include "Catalog.h"
#include "FulfillmentCenter.h"
#include "FulfillmentPlanner.h"
#include "Order.h"

namespace fixtures {

using namespace fulfillment;

//                                      SKU            weight g   volume cm3
inline const char* const KINDLE    = "B0KINDLE01";  //      205        700
inline const char* const ECHO_DOT  = "B0ECHODOT5";  //      304       1000
inline const char* const FIRE_TV   = "B0FIRETV4K";  //      150        300
inline const char* const SKILLET   = "B0BASICSKL";  //     3000       4000
inline const char* const WATER_24  = "B0WATER24P";  //     9000      12000
inline const char* const DUMBBELL  = "B0DUMBBELL";  //     9000       1500
inline const char* const DESKCHAIR = "B0DESKCHR1";  //    16000      90000

inline Catalog makeCatalog() {
    Catalog catalog;
    catalog.addItem(Item(KINDLE, "Kindle Paperwhite", 205, 700));
    catalog.addItem(Item(ECHO_DOT, "Echo Dot (5th Gen)", 304, 1000));
    catalog.addItem(Item(FIRE_TV, "Fire TV Stick 4K", 150, 300));
    catalog.addItem(Item(SKILLET, "Amazon Basics Cast Iron Skillet", 3000, 4000));
    catalog.addItem(Item(WATER_24, "Amazon Basics Spring Water, 24-pack", 9000, 12000));
    catalog.addItem(Item(DUMBBELL, "Amazon Basics 20 lb Dumbbell", 9000, 1500));
    catalog.addItem(Item(DESKCHAIR, "Amazon Basics Office Desk Chair", 16000, 90000));
    return catalog;
}

inline FulfillmentCenter makeCenter(int id, const std::string& code, Location loc,
                                    const std::vector<std::pair<std::string, int>>& stock) {
    FulfillmentCenter fc(id, code, loc);
    for (const auto& [sku, qty] : stock) {
        fc.inventory().addStock(sku, qty);
    }
    return fc;
}

inline Order standardOrder(int id, Location dest, std::vector<OrderLine> lines,
                           long long placedAt = 0) {
    return Order(id, dest, /*isPrime=*/false, placedAt, std::move(lines));
}

inline Order primeOrder(int id, Location dest, std::vector<OrderLine> lines,
                        long long placedAt = 0) {
    return Order(id, dest, /*isPrime=*/true, placedAt, std::move(lines));
}

}  // namespace fixtures
