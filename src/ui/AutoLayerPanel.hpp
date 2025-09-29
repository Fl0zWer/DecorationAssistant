#pragma once

#include <Geode/Geode.hpp>
#include "../core/LayerAssigner.hpp"
#include <optional>

namespace DecorationAssistant::UI {

class AutoLayerPanel : public CCLayerColor {
public:
    static AutoLayerPanel* create(LevelEditorLayer* editor);
    static void show(LevelEditorLayer* editor);

    bool init(LevelEditorLayer* editor);

private:
    LevelEditorLayer* m_editor = nullptr;
    DecorationAssistant::Core::LayerAssigner m_assigner;
    std::optional<DecorationAssistant::Core::LayerPreset> m_preset;

    void buildUI();
    void simulate();
    void apply();
};

} // namespace DecorationAssistant::UI
