#pragma once

#include <Geode/Geode.hpp>

namespace DecorationAssistant::Core {

struct AudioFeatures {
    bool hasDrop = false;
    std::vector<float> energy;
};

class AudioAnalyzer {
public:
    AudioFeatures analyze(LevelEditorLayer* layer) const;
};

} // namespace DecorationAssistant::Core
