#include "PaletteEngine.hpp"
#include "../utils/Json.hpp"
#include <Geode/utils/file.hpp>
#include <sstream>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

namespace {
ccColor4B parseColor(const std::string& hex) {
    unsigned value = 0;
    std::stringstream ss;
    ss << std::hex << hex.substr(hex[0] == '#' ? 1 : 0);
    ss >> value;
    return {
        static_cast<GLubyte>((value >> 16) & 0xFF),
        static_cast<GLubyte>((value >> 8) & 0xFF),
        static_cast<GLubyte>(value & 0xFF),
        255};
}
}

void PaletteEngine::load(const std::filesystem::path& path) {
    m_palettes.clear();
    auto raw = file::readString(path);
    if (!raw) {
        log::warn("PaletteEngine: missing {}", path.string());
        return;
    }
    auto json = Utils::jsonFromString(*raw);
    if (!json || !json->contains("palettes")) {
        log::warn("PaletteEngine: invalid file {}");
        return;
    }
    for (auto& entry : (*json)["palettes"]) {
        PaletteLibraryEntry pl;
        pl.name = entry.value("name", "Unnamed");
        pl.mood = entry.value("mood", "auto");
        pl.base = parseColor(entry.value("base", "#ffffff"));
        pl.contrastTarget = entry.value("contrastTarget", 0.5f);
        if (entry.contains("accents")) {
            for (auto& accent : entry["accents"]) {
                pl.accents.push_back(parseColor(accent.get<std::string>()));
            }
        }
        m_palettes.push_back(std::move(pl));
    }
    log::info("PaletteEngine loaded {} palettes", m_palettes.size());
}

std::optional<Palette> PaletteEngine::pick(const std::string& mood, float targetContrast) const {
    if (m_palettes.empty()) {
        return std::nullopt;
    }
    const auto* best = &m_palettes.front();
    float bestScore = std::numeric_limits<float>::max();
    for (auto& entry : m_palettes) {
        if (!mood.empty() && mood != "auto" && entry.mood != mood) {
            continue;
        }
        const float diff = std::abs(entry.contrastTarget - targetContrast);
        if (diff < bestScore) {
            bestScore = diff;
            best = &entry;
        }
    }
    Palette palette;
    palette.base = best->base;
    palette.accents = best->accents;
    palette.mood = best->mood;
    palette.contrast = best->contrastTarget;
    return palette;
}

float PaletteEngine::computeContrastScore(const Palette& palette) const {
    auto toVec = [](const ccColor4B& c) {
        return CCPoint{c.r / 255.f, c.g / 255.f};
    };
    CCPoint base = toVec(palette.base);
    float total = 0.f;
    for (auto& accent : palette.accents) {
        auto v = toVec(accent);
        total += ccpDistance(base, v);
    }
    if (palette.accents.empty()) {
        return 0.f;
    }
    return total / static_cast<float>(palette.accents.size());
}

} // namespace DecorationAssistant::Core
