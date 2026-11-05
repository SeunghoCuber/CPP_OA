#include "Catalog.h"

#include <stdexcept>

namespace fulfillment {

void Catalog::addItem(const Item& item) {
    auto [it, inserted] = items_.emplace(item.sku(), item);
    if (!inserted) {
        throw std::invalid_argument("Catalog: duplicate SKU " + item.sku());
    }
}

bool Catalog::contains(const std::string& sku) const {
    return items_.find(sku) != items_.end();
}

const Item& Catalog::get(const std::string& sku) const {
    auto it = items_.find(sku);
    if (it == items_.end()) {
        throw std::out_of_range("Catalog: unknown SKU " + sku);
    }
    return it->second;
}

}  // namespace fulfillment
