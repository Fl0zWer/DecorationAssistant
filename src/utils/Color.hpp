#pragma once

#include <Geode/Geode.hpp>

namespace DecorationAssistant::Utils {

ccColor4B withAlpha(const ccColor3B& color, GLubyte alpha);
ccColor4B fromHex(const std::string& hex);

} // namespace DecorationAssistant::Utils
