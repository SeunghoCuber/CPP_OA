#pragma once

#include <cstdlib>
#include <ostream>

namespace fulfillment {

// A point on the regional delivery grid. Units are "grid miles".
struct Location {
    int x = 0;
    int y = 0;
};

// Grid (Manhattan) distance between two locations: |dx| + |dy|.
inline int gridDistance(const Location& a, const Location& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

inline std::ostream& operator<<(std::ostream& os, const Location& loc) {
    return os << "(" << loc.x << ", " << loc.y << ")";
}

}  // namespace fulfillment
