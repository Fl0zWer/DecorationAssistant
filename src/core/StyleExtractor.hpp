#pragma once

#include <Geode/Geode.hpp>
#include "../integration/ImportBridge.hpp"

namespace DecorationAssistant::Core {

struct StyleProfile {
    float density = 0.5f;
    float contrast = 0.5f;
    float triggerComplexity = 0.5f;
    float symmetry = 0.5f;
};

class StyleExtractor {
public:
    StyleProfile fromLevel(const Integration::ImportedLevel& level) const;
    StyleProfile fromEditor(LevelEditorLayer* layer) const;
};

} // namespace DecorationAssistant::Core
