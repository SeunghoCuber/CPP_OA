#pragma once

#include <string>
#include <unordered_map>

namespace fulfillment {

// On-hand stock levels for a single fulfillment center, keyed by SKU.
class Inventory {
public:
    // Adds units to the stock of a SKU. Throws std::invalid_argument if quantity < 0.
    void addStock(const std::string& sku, int quantity);

    // Units currently available for the SKU (0 if the SKU has never been stocked).
    int available(const std::string& sku) const;

    // True if at least `quantity` units of the SKU are available.
    bool hasAtLeast(const std::string& sku, int quantity) const;

    // Removes `quantity` units from available stock.
    // Throws std::invalid_argument if quantity <= 0.
    // Throws std::runtime_error if fewer than `quantity` units are available
    // (in which case stock is left unchanged).
    void reserve(const std::string& sku, int quantity);

private:
    std::unordered_map<std::string, int> stock_;
};

}  // namespace fulfillment
