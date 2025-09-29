#pragma once

#include <Geode/Geode.hpp>
#include "../core/EditorScan.hpp"

namespace DecorationAssistant::UI {

class OverlayHud : public CCLayer {
public:
    static OverlayHud* create(const DecorationAssistant::Core::ScanResult& result);
    bool init(const DecorationAssistant::Core::ScanResult& result);
};

} // namespace DecorationAssistant::UI
