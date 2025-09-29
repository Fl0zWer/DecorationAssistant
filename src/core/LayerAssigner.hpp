#pragma once

#include <Geode/Geode.hpp>
#include "UndoManager.hpp"
#include <filesystem>

namespace DecorationAssistant::Core {

struct LayerAssignmentRule {
    std::string family;
    int layer = 0;
    int zOrder = 0;
    std::string detail;
    std::string groupPrefix;
    float throttle = 1.f;
};

struct LayerPreset {
    std::string name;
    std::vector<LayerAssignmentRule> rules;
    std::string conflictPolicy;
    std::pair<int, int> zBounds;
};

struct LayerPlanItem {
    GameObject* object = nullptr;
    int layer = 0;
    int zOrder = 0;
    std::string groupTag;
};

struct LayerPlan {
    std::string presetName;
    std::vector<LayerPlanItem> items;
};

class LayerAssigner {
public:
    void load(const std::filesystem::path& path);
    std::optional<LayerPreset> presetByName(const std::string& name) const;
    LayerPlan buildPlan(LevelEditorLayer* layer, const LayerPreset& preset, bool selectionOnly);
    void apply(LevelEditorLayer* layer, const LayerPlan& plan, bool previewOnly);

private:
    std::vector<LayerPreset> m_presets;
};

} // namespace DecorationAssistant::Core
