#pragma once

#include "StyleExtractor.hpp"
#include "DifficultyEstimator.hpp"
#include <filesystem>

namespace DecorationAssistant::Core {

struct CorpusLevel {
    std::string id;
    std::string name;
    StyleProfile profile;
    float difficulty = 0.f;
    std::vector<std::string> tags;
};

class LevelCorpus {
public:
    static LevelCorpus& get();

    void load(const std::filesystem::path& path);
    void save(const std::filesystem::path& path) const;

    void add(const CorpusLevel& level);
    std::vector<CorpusLevel> list() const;

private:
    std::vector<CorpusLevel> m_levels;
};

} // namespace DecorationAssistant::Core
