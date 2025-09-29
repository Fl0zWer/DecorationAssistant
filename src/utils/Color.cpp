#include "Color.hpp"

namespace DecorationAssistant::Utils {

ccColor4B withAlpha(const ccColor3B& color, GLubyte alpha) {
    return {color.r, color.g, color.b, alpha};
}

ccColor4B fromHex(const std::string& hex) {
    unsigned value = 0;
    std::stringstream ss;
    ss << std::hex << hex.substr(hex[0] == '#' ? 1 : 0);
    ss >> value;
    return {
        static_cast<GLubyte>((value >> 16) & 0xFF),
        static_cast<GLubyte>((value >> 8) & 0xFF),
        static_cast<GLubyte>(value & 0xFF),
        255};
}

} // namespace DecorationAssistant::Utils
