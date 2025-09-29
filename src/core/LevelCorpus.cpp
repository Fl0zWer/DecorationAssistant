#include "LevelCorpus.hpp"
#include "../utils/Json.hpp"
#include <Geode/utils/file.hpp>
#include <nlohmann/json.hpp>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

LevelCorpus& LevelCorpus::get() {
    static LevelCorpus instance;
    return instance;
}

void LevelCorpus::load(const std::filesystem::path& path) {
    m_levels.clear();
    auto raw = file::readString(path);
    if (!raw) {
        log::info("LevelCorpus: no index at {}", path.string());
        return;
    }
    auto json = Utils::jsonFromString(*raw);
    if (!json || !json->contains("levels")) {
        return;
    }
    for (auto& lv : (*json)["levels"]) {
        CorpusLevel level;
        level.id = lv.value("id", "unknown");
        level.name = lv.value("name", level.id);
        if (lv.contains("styleProfile")) {
            level.profile.density = lv["styleProfile"].value("density", 0.5f);
            level.profile.contrast = lv["styleProfile"].value("contrast", 0.5f);
            level.profile.triggerComplexity = lv["styleProfile"].value("triggerComplexity", 0.5f);
            level.profile.symmetry = lv["styleProfile"].value("symmetry", 0.5f);
        }
        level.difficulty = lv.value("difficulty", 0.5f);
        if (lv.contains("tags")) {
            for (auto& tag : lv["tags"]) {
                level.tags.push_back(tag.get<std::string>());
            }
        }
        m_levels.push_back(std::move(level));
    }
}

void LevelCorpus::save(const std::filesystem::path& path) const {
    nlohmann::json json;
    json["schemaVersion"] = 1;
    for (auto& level : m_levels) {
        nlohmann::json entry;
        entry["id"] = level.id;
        entry["name"] = level.name;
        entry["difficulty"] = level.difficulty;
        entry["styleProfile"] = {
            {"density", level.profile.density},
            {"contrast", level.profile.contrast},
            {"triggerComplexity", level.profile.triggerComplexity},
            {"symmetry", level.profile.symmetry}
        };
        entry["tags"] = level.tags;
        json["levels"].push_back(entry);
    }
    file::writeString(path, json.dump(2));
}

void LevelCorpus::add(const CorpusLevel& level) {
    m_levels.push_back(level);
}

std::vector<CorpusLevel> LevelCorpus::list() const {
    return m_levels;
}

} // namespace DecorationAssistant::Core
