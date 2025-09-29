#pragma once

#include <Geode/Geode.hpp>
#include <functional>

namespace DecorationAssistant::Integration {

struct EditorContext {
    LevelEditorLayer* editorLayer = nullptr;
    CCArrayExt<GameObject*> selection;
    bool isValid() const { return editorLayer != nullptr; }
};

class EditorAdapter {
public:
    static EditorAdapter& get();

    void bind(LevelEditorLayer* layer);
    EditorContext context() const;

    CCPoint screenToLayer(const CCPoint& pt) const;
    void runOnMainThread(std::function<void()> fn) const;

private:
    LevelEditorLayer* m_layer = nullptr;
};

} // namespace DecorationAssistant::Integration
