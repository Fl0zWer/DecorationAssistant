#pragma once

#include "Suggestion.hpp"
#include <filesystem>

namespace DecorationAssistant::Core {

struct PaletteLibraryEntry {
    std::string name;
    ccColor4B base;
    std::vector<ccColor4B> accents;
    std::string mood;
    float contrastTarget = 0.f;
};

class PaletteEngine {
public:
    void load(const std::filesystem::path& path);
    std::optional<Palette> pick(const std::string& mood, float targetContrast) const;
    float computeContrastScore(const Palette& palette) const;

private:
    std::vector<PaletteLibraryEntry> m_palettes;
};

} // namespace DecorationAssistant::Core
