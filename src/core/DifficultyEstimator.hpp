#pragma once

#include "StyleExtractor.hpp"

namespace DecorationAssistant::Core {

class DifficultyEstimator {
public:
    float estimate(const Integration::ImportedLevel& level) const;
    float estimate(LevelEditorLayer* layer) const;
};

} // namespace DecorationAssistant::Core
