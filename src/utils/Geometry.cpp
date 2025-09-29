#include "Geometry.hpp"

namespace DecorationAssistant::Utils {

float distance2D(const CCPoint& a, const CCPoint& b) {
    return ccpDistance(a, b);
}

CCRect inflate(const CCRect& rect, float padding) {
    return {rect.origin.x - padding, rect.origin.y - padding, rect.size.width + padding * 2.f, rect.size.height + padding * 2.f};
}

} // namespace DecorationAssistant::Utils
