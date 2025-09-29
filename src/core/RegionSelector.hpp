#pragma once

#include <Geode/Geode.hpp>

namespace DecorationAssistant::Core {

class RegionSelector {
public:
    CCRect selectionBounds(LevelEditorLayer* layer, bool selectionOnly) const;
};

} // namespace DecorationAssistant::Core
