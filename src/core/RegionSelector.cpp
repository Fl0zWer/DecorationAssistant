#include "RegionSelector.hpp"

using namespace geode::prelude;

namespace DecorationAssistant::Core {

CCRect RegionSelector::selectionBounds(LevelEditorLayer* layer, bool selectionOnly) const {
    if (!layer) {
        return {0, 0, 0, 0};
    }
    auto array = selectionOnly ? CCArrayExt<GameObject*>(layer->getSelectedObjects()) : CCArrayExt<GameObject*>(layer->getAllObjects());
    if (array.empty()) {
        return layer->getObjectLayer()->boundingBox();
    }
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    for (auto* obj : array) {
        if (!obj) continue;
        auto pos = obj->getPosition();
        minX = std::min(minX, pos.x);
        minY = std::min(minY, pos.y);
        maxX = std::max(maxX, pos.x);
        maxY = std::max(maxY, pos.y);
    }
    if (minX > maxX || minY > maxY) {
        return {0, 0, 0, 0};
    }
    return {minX, minY, maxX - minX, maxY - minY};
}

} // namespace DecorationAssistant::Core
