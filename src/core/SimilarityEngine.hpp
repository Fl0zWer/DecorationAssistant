#pragma once

#include "LevelCorpus.hpp"

namespace DecorationAssistant::Core {

class SimilarityEngine {
public:
    std::vector<CorpusLevel> topK(const StyleProfile& profile, size_t k) const;
};

} // namespace DecorationAssistant::Core
