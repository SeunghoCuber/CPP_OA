#include "FulfillmentCenter.h"

#include <stdexcept>
#include <utility>

namespace fulfillment {

FulfillmentCenter::FulfillmentCenter(int id, std::string code, Location location)
    : id_(id), code_(std::move(code)), location_(location) {
    if (code_.empty()) {
        throw std::invalid_argument("FulfillmentCenter: code must not be empty");
    }
}

}  // namespace fulfillment
