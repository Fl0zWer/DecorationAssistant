#include "RuleEngine.hpp"
#include "../utils/Json.hpp"
#include <Geode/utils/file.hpp>
#include <nlohmann/json.hpp>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

void RuleEngine::loadFromFile(const std::filesystem::path& path) {
    m_rules.clear();
    auto raw = file::readString(path);
    if (!raw) {
        log::warn("RuleEngine: could not read {}", path.string());
        return;
    }
    auto json = Utils::jsonFromString(*raw);
    if (!json) {
        log::warn("RuleEngine: invalid JSON {}");
        return;
    }
    if (!json->contains("rules")) {
        log::warn("RuleEngine: no rules field");
        return;
    }
    for (auto& rule : (*json)["rules"]) {
        RuleDefinition def;
        def.id = rule.value("id", "anonymous");
        def.json = rule;
        m_rules.push_back(std::move(def));
    }
    log::info("RuleEngine loaded {} rules", m_rules.size());
}

std::vector<Suggestion> RuleEngine::evaluate(const RuleContext& ctx, size_t limit) const {
    std::vector<Suggestion> out;
    if (ctx.scan.cells.empty()) {
        return out;
    }
    for (auto& def : m_rules) {
        if (out.size() >= limit) break;
        const auto suggest = def.json.contains("suggest") ? def.json["suggest"] : nlohmann::json::object();
        Suggestion suggestion;
        suggestion.type = SuggestionType::Border;
        suggestion.region = ctx.scan.cells.front().region;
        suggestion.priority = suggest.value("priority", 0.5f);
        suggestion.confidence = suggest.value("confidence", 0.5f);
        suggestion.rationale = suggest.value("rationale", "Regla placeholder");
        out.push_back(std::move(suggestion));
    }
    return out;
}

} // namespace DecorationAssistant::Core
