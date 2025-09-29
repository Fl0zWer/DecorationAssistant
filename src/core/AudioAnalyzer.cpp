#include "AudioAnalyzer.hpp"

using namespace geode::prelude;

namespace DecorationAssistant::Core {

AudioFeatures AudioAnalyzer::analyze(LevelEditorLayer* layer) const {
    AudioFeatures features;
    if (!layer) {
        return features;
    }
    // TODO: integrate with actual audio timeline
    features.hasDrop = true;
    features.energy = {0.1f, 0.4f, 0.9f};
    return features;
}

} // namespace DecorationAssistant::Core
