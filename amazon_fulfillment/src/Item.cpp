#include "Item.h"

#include <stdexcept>
#include <utility>

namespace fulfillment {

Item::Item(std::string sku, std::string title, int weightGrams, int volumeCm3)
    : sku_(std::move(sku)),
      title_(std::move(title)),
      weightGrams_(weightGrams),
      volumeCm3_(volumeCm3) {
    if (sku_.empty()) {
        throw std::invalid_argument("Item: SKU must not be empty");
    }
    if (weightGrams_ <= 0) {
        throw std::invalid_argument("Item " + sku_ + ": weight must be positive");
    }
    if (volumeCm3_ <= 0) {
        throw std::invalid_argument("Item " + sku_ + ": volume must be positive");
    }
}

}  // namespace fulfillment
