#include "Inventory.h"

#include <stdexcept>

namespace fulfillment {

void Inventory::addStock(const std::string& sku, int quantity) {
    if (quantity < 0) {
        throw std::invalid_argument("Inventory: cannot add negative stock for " + sku);
    }
    stock_[sku] += quantity;
}

int Inventory::available(const std::string& sku) const {
    auto it = stock_.find(sku);
    return it == stock_.end() ? 0 : it->second;
}

bool Inventory::hasAtLeast(const std::string& sku, int quantity) const {
    return available(sku) >= quantity;
}

void Inventory::reserve(const std::string& sku, int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Inventory: reserve quantity must be positive for " + sku);
    }
    auto it = stock_.find(sku);
    if (it == stock_.end() || it->second < quantity) {
        throw std::runtime_error("Inventory: insufficient stock for " + sku);
    }
    it->second -= quantity;
}

}  // namespace fulfillment
