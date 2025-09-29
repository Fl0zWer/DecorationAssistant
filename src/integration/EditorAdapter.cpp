#include "EditorAdapter.hpp"
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

namespace DecorationAssistant::Integration {

EditorAdapter& EditorAdapter::get() {
    static EditorAdapter instance;
    return instance;
}

void EditorAdapter::bind(LevelEditorLayer* layer) {
    m_layer = layer;
}

EditorContext EditorAdapter::context() const {
    EditorContext ctx;
    ctx.editorLayer = m_layer;
    if (m_layer) {
        ctx.selection = CCArrayExt<GameObject*>(m_layer->getSelectedObjects());
    }
    return ctx;
}

CCPoint EditorAdapter::screenToLayer(const CCPoint& pt) const {
    if (!m_layer) {
        return pt;
    }
    return m_layer->getObjectLayer()->convertToNodeSpace(pt);
}

void EditorAdapter::runOnMainThread(std::function<void()> fn) const {
    CCDirector::get()->getScheduler()->schedule(
        [fn = std::move(fn)](float) {
            if (fn) fn();
        },
        CCDirector::get(),
        0.f,
        0,
        0.f,
        false,
        "DecorationAdapterInvoke");
}

} // namespace DecorationAssistant::Integration
