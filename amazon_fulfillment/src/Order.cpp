#include "Order.h"

#include <stdexcept>
#include <string>
#include <utility>

namespace fulfillment {

Order::Order(int id, Location destination, bool isPrime, long long placedAt,
             std::vector<OrderLine> lines)
    : id_(id),
      destination_(destination),
      isPrime_(isPrime),
      placedAt_(placedAt),
      lines_(std::move(lines)) {
    if (lines_.empty()) {
        throw std::invalid_argument("Order " + std::to_string(id_) + ": must have at least one line");
    }
    for (const auto& line : lines_) {
        if (line.quantity <= 0) {
            throw std::invalid_argument("Order " + std::to_string(id_) +
                                        ": line quantity must be positive for " + line.sku);
        }
    }
}

int Order::totalUnits() const {
    int total = 0;
    for (const auto& line : lines_) {
        total += line.quantity;
    }
    return total;
}

}  // namespace fulfillment
