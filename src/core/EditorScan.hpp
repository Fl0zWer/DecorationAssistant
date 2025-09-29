#pragma once

#include <Geode/Geode.hpp>
#include "Suggestion.hpp"

namespace DecorationAssistant::Core {

struct ScanCell {
    Region region;
    float structuralDensity = 0.f;
    float decorativeDensity = 0.f;
    float colorVariance = 0.f;
    bool hasPortal = false;
};

struct ScanResult {
    std::vector<ScanCell> cells;
    float averageDensity = 0.f;
    CCRect bounds;
};

class EditorScan {
public:
    ScanResult scan(LevelEditorLayer* layer, const CCRect& area, size_t grid = 8);
};

} // namespace DecorationAssistant::Core
