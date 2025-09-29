#include "SimilarityEngine.hpp"
#include <cmath>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

namespace {
float cosine(const StyleProfile& a, const StyleProfile& b) {
    const float dot = a.density * b.density + a.contrast * b.contrast + a.triggerComplexity * b.triggerComplexity + a.symmetry * b.symmetry;
    const float normA = std::sqrt(a.density * a.density + a.contrast * a.contrast + a.triggerComplexity * a.triggerComplexity + a.symmetry * a.symmetry);
    const float normB = std::sqrt(b.density * b.density + b.contrast * b.contrast + b.triggerComplexity * b.triggerComplexity + b.symmetry * b.symmetry);
    if (normA == 0 || normB == 0) {
        return 0.f;
    }
    return dot / (normA * normB);
}
}

std::vector<CorpusLevel> SimilarityEngine::topK(const StyleProfile& profile, size_t k) const {
    auto levels = LevelCorpus::get().list();
    std::vector<std::pair<float, CorpusLevel>> scored;
    for (auto& level : levels) {
        scored.emplace_back(cosine(profile, level.profile), level);
    }
    std::sort(scored.begin(), scored.end(), [](auto& lhs, auto& rhs) {
        return lhs.first > rhs.first;
    });
    std::vector<CorpusLevel> out;
    for (size_t i = 0; i < std::min(k, scored.size()); ++i) {
        out.push_back(scored[i].second);
    }
    return out;
}

} // namespace DecorationAssistant::Core
