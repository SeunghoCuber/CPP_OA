#pragma once

#include <string>

#include "Inventory.h"
#include "Location.h"

namespace fulfillment {

// An Amazon fulfillment center (FC), e.g. "SEA1", with its own inventory.
class FulfillmentCenter {
public:
    // Throws std::invalid_argument if code is empty.
    FulfillmentCenter(int id, std::string code, Location location);

    int id() const { return id_; }
    const std::string& code() const { return code_; }
    const Location& location() const { return location_; }

    Inventory& inventory() { return inventory_; }
    const Inventory& inventory() const { return inventory_; }

private:
    int id_;
    std::string code_;
    Location location_;
    Inventory inventory_;
};

}  // namespace fulfillment
