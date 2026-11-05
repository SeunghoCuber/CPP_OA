#pragma once

#include <ostream>
#include <vector>

namespace fulfillment {

// Standard Amazon box sizes, smallest to largest.
enum class BoxSize { Small, Medium, Large, Freight };

inline const char* toString(BoxSize box) {
    switch (box) {
        case BoxSize::Small: return "Small";
        case BoxSize::Medium: return "Medium";
        case BoxSize::Large: return "Large";
        case BoxSize::Freight: return "Freight";
    }
    return "Unknown";
}

inline std::ostream& operator<<(std::ostream& os, BoxSize box) {
    return os << toString(box);
}

// The plan for shipping a single order out of a single fulfillment center.
struct Shipment {
    int orderId = 0;
    int fulfillmentCenterId = 0;
    int distance = 0;
    int deliveryDays = 0;
    BoxSize box = BoxSize::Small;
    long long shippingCostCents = 0;
};

// Result of planning a batch of orders.
struct BatchPlan {
    std::vector<Shipment> shipments;         // in processing order
    std::vector<int> backorderedOrderIds;    // in processing order
};

}  // namespace fulfillment
