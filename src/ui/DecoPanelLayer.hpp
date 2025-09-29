#pragma once

#include <Geode/Geode.hpp>
#include "../core/RuleEngine.hpp"
#include "../core/PaletteEngine.hpp"
#include "../core/ApplyEngine.hpp"
#include "../core/EditorScan.hpp"
#include "../core/AudioAnalyzer.hpp"
#include "../core/RegionSelector.hpp"
#include "CorpusPanel.hpp"

namespace DecorationAssistant::UI {

class DecoPanelLayer : public CCLayerColor {
public:
    static DecoPanelLayer* create(LevelEditorLayer* editor);
    static void toggle(LevelEditorLayer* editor);

    bool init(LevelEditorLayer* editor);

private:
    LevelEditorLayer* m_editor = nullptr;
    DecorationAssistant::Core::RuleEngine m_rules;
    DecorationAssistant::Core::PaletteEngine m_palettes;
    DecorationAssistant::Core::ApplyEngine m_apply;
    DecorationAssistant::Core::EditorScan m_scan;
    DecorationAssistant::Core::AudioAnalyzer m_audio;
    DecorationAssistant::Core::RegionSelector m_selector;

    void buildUI();
    void refreshSuggestions();
};

} // namespace DecorationAssistant::UI
