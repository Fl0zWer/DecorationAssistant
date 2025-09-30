#pragma once

#include "Suggestion.hpp"
#include "EditorScan.hpp"
#include <filesystem>
#include <optional>
#include <random>
#include <nlohmann/json_fwd.hpp>

namespace DecorationAssistant::Core {

struct RuleContext {
    ScanResult scan;
    std::optional<Palette> palette;
    std::string styleHint;
    float contrastTarget = 0.f;
    bool audioDrop = false;
    std::mt19937 rng;
};

class RuleEngine {
public:
    void loadFromFile(const std::filesystem::path& path);
    std::vector<Suggestion> evaluate(const RuleContext& ctx, size_t limit = 6) const;

private:
    struct RuleDefinition {
        std::string id;
        nlohmann::json json;
    };

    std::vector<RuleDefinition> m_rules;
};

} // namespace DecorationAssistant::Core
