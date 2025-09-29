#include "StyleExtractor.hpp"
#include "EditorScan.hpp"
#include <algorithm>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

StyleProfile StyleExtractor::fromLevel(const Integration::ImportedLevel& level) const {
    StyleProfile profile;
    profile.density = std::min(1.f, static_cast<float>(level.objects.size()) / 2000.f);
    profile.contrast = 0.7f;
    profile.triggerComplexity = 0.6f;
    profile.symmetry = 0.4f;
    return profile;
}

StyleProfile StyleExtractor::fromEditor(LevelEditorLayer* layer) const {
    StyleProfile profile;
    if (!layer) {
        return profile;
    }
    EditorScan scan;
    auto bounds = layer->getObjectLayer()->boundingBox();
    auto result = scan.scan(layer, bounds, 6);
    profile.density = result.averageDensity;
    profile.contrast = 0.6f;
    profile.triggerComplexity = 0.5f;
    profile.symmetry = 0.5f;
    return profile;
}

} // namespace DecorationAssistant::Core
