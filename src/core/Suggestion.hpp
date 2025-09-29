#pragma once

#include <Geode/Geode.hpp>
#include <optional>
#include <unordered_map>
#include <vector>
#include <string>

namespace DecorationAssistant::Core {

enum class SuggestionType {
    Border,
    BackgroundDetail,
    Particle,
    Pulse,
    Glow,
    ThemeElement,
    Trigger
};

struct Region {
    CCRect bounds;
    float density = 0.f;
    std::string label;
};

struct Palette {
    ccColor4B base;
    std::vector<ccColor4B> accents;
    float contrast = 0.f;
    std::string mood;
};

struct Suggestion {
    SuggestionType type;
    Region region;
    std::optional<Palette> palette;
    float priority = 0.f;
    float confidence = 0.f;
    std::string rationale;
    std::unordered_map<std::string, float> numericParams;
    std::unordered_map<std::string, std::string> stringParams;
};

} // namespace DecorationAssistant::Core
