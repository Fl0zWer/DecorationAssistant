#include "LayerAssigner.hpp"
#include "../utils/Json.hpp"
#include <Geode/utils/file.hpp>
#include <fmt/format.h>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

void LayerAssigner::load(const std::filesystem::path& path) {
    m_presets.clear();
    auto raw = file::readString(path);
    if (!raw) {
        log::warn("LayerAssigner: missing presets {}", path.string());
        return;
    }
    auto json = Utils::jsonFromString(*raw);
    if (!json || !json->contains("presets")) {
        return;
    }
    for (auto& preset : (*json)["presets"]) {
        LayerPreset lp;
        lp.name = preset.value("name", "Unnamed");
        lp.conflictPolicy = preset.value("conflictPolicy", "last-wins");
        lp.zBounds = {preset["zOrderBounds"].value("min", -10), preset["zOrderBounds"].value("max", 20)};
        for (auto& rule : preset["rules"]) {
            LayerAssignmentRule lr;
            lr.family = rule.value("family", "Generic");
            lr.layer = rule.value("layer", 0);
            lr.zOrder = rule.value("zOrder", 0);
            lr.detail = rule.value("detail", "mid");
            lr.groupPrefix = rule.value("groupPrefix", "");
            lr.throttle = rule.value("throttle", 1.f);
            lp.rules.push_back(lr);
        }
        m_presets.push_back(std::move(lp));
    }
}

std::optional<LayerPreset> LayerAssigner::presetByName(const std::string& name) const {
    auto it = std::find_if(m_presets.begin(), m_presets.end(), [&](auto& preset) {
        return preset.name == name;
    });
    if (it == m_presets.end()) {
        return std::nullopt;
    }
    return *it;
}

LayerPlan LayerAssigner::buildPlan(LevelEditorLayer* layer, const LayerPreset& preset, bool selectionOnly) {
    LayerPlan plan;
    plan.presetName = preset.name;
    if (!layer || preset.rules.empty()) {
        return plan;
    }
    auto objs = selectionOnly ? CCArrayExt<GameObject*>(layer->getSelectedObjects()) : CCArrayExt<GameObject*>(layer->getAllObjects());
    size_t index = 0;
    for (auto* obj : objs) {
        if (!obj) continue;
        const auto& rule = preset.rules[index % preset.rules.size()];
        if (rule.throttle < 1.f) {
            float ratio = static_cast<float>((index % 100)) / 100.f;
            if (ratio >= rule.throttle) {
                ++index;
                continue;
            }
        }
        LayerPlanItem item;
        item.object = obj;
        item.layer = rule.layer;
        item.zOrder = std::clamp(rule.zOrder, preset.zBounds.first, preset.zBounds.second);
        if (!rule.groupPrefix.empty()) {
            item.groupTag = fmt::format("{}_{}", rule.groupPrefix, index);
        }
        plan.items.push_back(item);
        ++index;
    }
    return plan;
}

void LayerAssigner::apply(LevelEditorLayer* layer, const LayerPlan& plan, bool previewOnly) {
    if (!layer) return;
    auto composite = std::make_shared<CompositeCommand>(fmt::format("AutoLayer: {}", plan.presetName));
    for (auto& item : plan.items) {
        auto cmd = std::make_shared<ModifyPropsCommand>();
        cmd->label = fmt::format("Layer {}", plan.presetName);
        composite->commands.push_back(cmd);
        if (!previewOnly && item.object) {
            item.object->setZOrder(item.zOrder);
        }
    }
    UndoManager::get().push(composite);
}

} // namespace DecorationAssistant::Core
