#pragma once

#include <string>

namespace fulfillment {

// A sellable product identified by its SKU (ASIN-style identifier).
// Weight and volume describe a single unit of the product.
class Item {
public:
    // Throws std::invalid_argument if sku is empty or weight/volume are not positive.
    Item(std::string sku, std::string title, int weightGrams, int volumeCm3);

    const std::string& sku() const { return sku_; }
    const std::string& title() const { return title_; }
    int weightGrams() const { return weightGrams_; }
    int volumeCm3() const { return volumeCm3_; }

private:
    std::string sku_;
    std::string title_;
    int weightGrams_;
    int volumeCm3_;
};

}  // namespace fulfillment
