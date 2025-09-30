#include "UndoManager.hpp"
#include <Geode/binding/GameObject.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

CreateObjectsCommand::~CreateObjectsCommand() {
    for (auto* obj : created) {
        if (obj) {
            obj->release();
        }
    }
}

void CreateObjectsCommand::apply() {
    if (!layer) {
        return;
    }
    for (auto* obj : created) {
        if (!obj) continue;
        if (obj->getParent()) {
            continue;
        }
        layer->addObject(obj);
    }
}

void CreateObjectsCommand::revert() {
    if (!layer) {
        return;
    }
    for (auto* obj : created) {
        if (!obj) continue;
        if (!obj->getParent()) {
            continue;
        }
        layer->removeObject(obj, true);
    }
}

} // namespace DecorationAssistant::Core

