#include "OverlayHud.hpp"

using namespace geode::prelude;

namespace DecorationAssistant::UI {

OverlayHud* OverlayHud::create(const DecorationAssistant::Core::ScanResult& result) {
    auto layer = new OverlayHud();
    if (layer && layer->init(result)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool OverlayHud::init(const DecorationAssistant::Core::ScanResult& result) {
    if (!CCLayer::init()) {
        return false;
    }
    for (auto& cell : result.cells) {
        auto node = CCDrawNode::create();
        ccColor4F color = {cell.region.density, 1.f - cell.region.density, 0.f, 0.3f};
        node->drawRect(cell.region.bounds.origin, cell.region.bounds.origin + cell.region.bounds.size, color);
        this->addChild(node);
    }
    return true;
}

} // namespace DecorationAssistant::UI
