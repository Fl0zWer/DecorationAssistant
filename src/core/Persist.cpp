#include "Persist.hpp"
#include <Geode/utils/file.hpp>
#include <nlohmann/json.hpp>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

Persist& Persist::get() {
    static Persist instance;
    return instance;
}

void Persist::load() {
    auto raw = file::readString("profile.json");
    if (!raw) {
        return;
    }
    auto json = nlohmann::json::parse(*raw, nullptr, false);
    if (json.is_discarded()) {
        return;
    }
    m_state.lastPreset = json.value("lastPreset", "Clean-Tech");
    if (json.contains("recentReferences")) {
        for (auto& entry : json["recentReferences"]) {
            m_state.recentReferences.push_back(entry.get<std::string>());
        }
    }
}

void Persist::flush() {
    nlohmann::json json;
    json["lastPreset"] = m_state.lastPreset;
    json["recentReferences"] = m_state.recentReferences;
    file::writeString("profile.json", json.dump(2));
}

} // namespace DecorationAssistant::Core
