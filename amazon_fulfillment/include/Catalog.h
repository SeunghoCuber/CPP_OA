#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

#include "Item.h"

namespace fulfillment {

// The product catalog: the single source of truth for per-unit item data.
class Catalog {
public:
    // Throws std::invalid_argument if an item with the same SKU already exists.
    void addItem(const Item& item);

    bool contains(const std::string& sku) const;

    // Throws std::out_of_range if the SKU is not in the catalog.
    const Item& get(const std::string& sku) const;

    std::size_t size() const { return items_.size(); }

private:
    std::unordered_map<std::string, Item> items_;
};

}  // namespace fulfillment
