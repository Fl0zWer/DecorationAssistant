#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/loader/SettingEvent.hpp>

namespace DecorationAssistant::Integration {

void installHooks();

class EditorLayerHook final : public geode::Modify<EditorLayerHook, EditorPauseLayer> {
public:
    bool init(LevelEditorLayer* editor) override;
};

} // namespace DecorationAssistant::Integration
