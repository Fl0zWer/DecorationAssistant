#include "ImportBridge.hpp"
#include "../utils/Json.hpp"
#include <Geode/utils/file.hpp>

using namespace geode::prelude;

namespace DecorationAssistant::Integration {

ImportBridge& ImportBridge::get() {
    static ImportBridge instance;
    return instance;
}

std::optional<ImportedLevel> ImportBridge::importFromFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        log::warn("ImportBridge: file not found {}", path.string());
        return std::nullopt;
    }
    auto raw = file::readString(path);
    if (!raw) {
        log::warn("ImportBridge: failed to read {}", path.string());
        return std::nullopt;
    }
    auto json = Utils::jsonFromString(*raw);
    if (!json) {
        log::warn("ImportBridge: invalid json {}", path.string());
        return std::nullopt;
    }
    ImportedLevel level;
    level.id = json->value("id", path.stem().string());
    level.name = json->value("name", level.id);
    level.bpm = json->value("bpm", 0.0);
    if (json->contains("tags")) {
        for (auto& tag : (*json)["tags"]) {
            level.tags.push_back(tag.get<std::string>());
        }
    }
    // TODO: map objects into runtime objects; placeholder empties
    return level;
}

std::optional<ImportedLevel> ImportBridge::importFromClipboard() const {
    auto text = Clipboard::get();
    if (!text || text->empty()) {
        return std::nullopt;
    }
    auto json = Utils::jsonFromString(*text);
    if (!json) {
        return std::nullopt;
    }
    ImportedLevel level;
    level.id = json->value("id", "clipboard-level");
    level.name = json->value("name", level.id);
    level.bpm = json->value("bpm", 0.0);
    if (json->contains("tags")) {
        for (auto& tag : (*json)["tags"]) {
            level.tags.push_back(tag.get<std::string>());
        }
    }
    return level;
}

} // namespace DecorationAssistant::Integration
