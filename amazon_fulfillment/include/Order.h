#pragma once

#include <string>
#include <vector>

#include "Location.h"

namespace fulfillment {

// One line of a customer order. The same SKU MAY appear on more than one line.
struct OrderLine {
    std::string sku;
    int quantity = 0;
};

// A customer order waiting to be fulfilled.
class Order {
public:
    // Throws std::invalid_argument if `lines` is empty or any line has quantity <= 0.
    Order(int id, Location destination, bool isPrime, long long placedAt,
          std::vector<OrderLine> lines);

    int id() const { return id_; }
    const Location& destination() const { return destination_; }
    bool isPrime() const { return isPrime_; }

    // Time the order was placed, in seconds since an arbitrary epoch.
    long long placedAt() const { return placedAt_; }

    const std::vector<OrderLine>& lines() const { return lines_; }

    // Sum of quantities across all lines.
    int totalUnits() const;

private:
    int id_;
    Location destination_;
    bool isPrime_;
    long long placedAt_;
    std::vector<OrderLine> lines_;
};

}  // namespace fulfillment
