#include "DifficultyEstimator.hpp"
#include <algorithm>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

float DifficultyEstimator::estimate(const Integration::ImportedLevel& level) const {
    return std::clamp(level.objects.size() / 4000.f, 0.f, 1.f);
}

float DifficultyEstimator::estimate(LevelEditorLayer* layer) const {
    if (!layer) {
        return 0.0f;
    }
    auto totalObjects = layer->getAllObjects()->count();
    return std::clamp(static_cast<float>(totalObjects) / 4000.f, 0.f, 1.f);
}

} // namespace DecorationAssistant::Core
