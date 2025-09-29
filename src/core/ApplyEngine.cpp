#include "ApplyEngine.hpp"
#include "UndoManager.hpp"
#include <memory>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

ApplyReport ApplyEngine::apply(LevelEditorLayer* layer, const std::vector<Suggestion>& suggestions, bool previewOnly) {
    ApplyReport report;
    if (!layer || suggestions.empty()) {
        return report;
    }

    auto composite = std::make_shared<CompositeCommand>("Autodeco batch");
    for (auto& suggestion : suggestions) {
        auto cmd = std::make_shared<CreateObjectsCommand>();
        cmd->label = suggestion.rationale;
        if (!previewOnly) {
            report.createdObjects += 1;
        }
        composite->commands.push_back(cmd);
    }
    UndoManager::get().push(composite);
    report.label = composite->label;
    return report;
}

} // namespace DecorationAssistant::Core
