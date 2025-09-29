#pragma once

#include <Geode/Geode.hpp>
#include "../core/LevelCorpus.hpp"
#include "../core/SimilarityEngine.hpp"

namespace DecorationAssistant::UI {

class CorpusPanel : public CCLayer {
public:
    static CorpusPanel* create();
    bool init();

    void refresh();
};

} // namespace DecorationAssistant::UI
