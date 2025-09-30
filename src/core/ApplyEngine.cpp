#include "ApplyEngine.hpp"
#include "UndoManager.hpp"
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <algorithm>
#include <cmath>
#include <fmt/format.h>
#include <memory>
#include <optional>
#include <string_view>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

namespace {

int suggestionDefaultObjectID(SuggestionType type) {
    switch (type) {
        case SuggestionType::Border:
            return 1; // simple block
        case SuggestionType::BackgroundDetail:
            return 744; // decorative detail
        case SuggestionType::Particle:
            return 2065; // particle emitter
        case SuggestionType::Pulse:
            return 1006; // color trigger
        case SuggestionType::Glow:
            return 841; // glow piece
        case SuggestionType::ThemeElement:
            return 2903; // themed deco
        case SuggestionType::Trigger:
            return 1000; // generic trigger
    }
    return 1;
}

std::optional<float> findNumeric(const Suggestion& suggestion, std::string_view key) {
    auto it = suggestion.numericParams.find(std::string(key));
    if (it != suggestion.numericParams.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<int> findIntNumeric(const Suggestion& suggestion, std::string_view key) {
    if (auto value = findNumeric(suggestion, key)) {
        return static_cast<int>(std::lround(*value));
    }
    return std::nullopt;
}

std::optional<int> findIntString(const Suggestion& suggestion, std::string_view key) {
    auto it = suggestion.stringParams.find(std::string(key));
    if (it == suggestion.stringParams.end()) {
        return std::nullopt;
    }
    try {
        return std::stoi(it->second);
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<GameObject*> buildObjectsForSuggestion(const Suggestion& suggestion) {
    std::vector<GameObject*> objects;

    int objectID = suggestionDefaultObjectID(suggestion.type);
    if (auto id = findIntNumeric(suggestion, "object_id")) {
        objectID = *id;
    } else if (auto id = findIntNumeric(suggestion, "objectID")) {
        objectID = *id;
    } else if (auto id = findIntString(suggestion, "object_id")) {
        objectID = *id;
    } else if (auto id = findIntString(suggestion, "objectID")) {
        objectID = *id;
    } else if (auto id = findIntString(suggestion, "object")) {
        objectID = *id;
    }

    auto* object = GameObject::createWithKey(objectID);
    if (!object) {
        return objects;
    }

    const auto bounds = suggestion.region.bounds;
    float posX = bounds.getMidX();
    float posY = bounds.getMidY();
    if (auto val = findNumeric(suggestion, "x")) posX = *val;
    if (auto val = findNumeric(suggestion, "y")) posY = *val;
    object->setPosition({posX, posY});

    if (auto val = findNumeric(suggestion, "scale")) object->setScale(*val);
    if (auto val = findNumeric(suggestion, "scaleX")) object->setScaleX(*val);
    if (auto val = findNumeric(suggestion, "scaleY")) object->setScaleY(*val);
    if (auto val = findNumeric(suggestion, "rotation")) object->setRotation(*val);
    if (auto val = findNumeric(suggestion, "opacity")) object->setOpacity(static_cast<GLubyte>(std::clamp(*val, 0.f, 255.f)));

    if (suggestion.palette) {
        const auto& base = suggestion.palette->base;
        object->setColor({base.r, base.g, base.b});
        object->setOpacity(base.a);
    }

    objects.push_back(object);
    return objects;
}

} // namespace

ApplyReport ApplyEngine::apply(LevelEditorLayer* layer, const std::vector<Suggestion>& suggestions, bool previewOnly) {
    ApplyReport report;
    if (!layer || suggestions.empty()) {
        return report;
    }

    if (previewOnly) {
        report.label = "Autodeco preview";
        return report;
    }

    auto composite = std::make_shared<CompositeCommand>("Autodeco batch");
    for (auto& suggestion : suggestions) {
        auto objects = buildObjectsForSuggestion(suggestion);
        if (objects.empty()) {
            continue;
        }

        auto cmd = std::make_shared<CreateObjectsCommand>();
        cmd->label = suggestion.rationale.empty() ? "Crear objeto" : suggestion.rationale;
        cmd->layer = layer;

        for (auto* obj : objects) {
            if (!obj) continue;
            obj->retain();
            cmd->created.push_back(obj);
        }

        if (cmd->created.empty()) {
            continue;
        }

        cmd->apply();
        report.createdObjects += cmd->created.size();
        composite->commands.push_back(cmd);
    }

    if (composite->commands.empty()) {
        return report;
    }

    if (report.createdObjects > 0) {
        composite->label = fmt::format("Autodeco: {} objetos", report.createdObjects);
    }
    UndoManager::get().push(composite);
    report.label = composite->label;
    return report;
}

} // namespace DecorationAssistant::Core
