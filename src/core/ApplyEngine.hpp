#pragma once

#include "Suggestion.hpp"
#include "UndoManager.hpp"
#include <vector>
#include <string>

namespace DecorationAssistant::Core {

struct ApplyReport {
    size_t createdObjects = 0;
    size_t modifiedObjects = 0;
    std::string label;
};

class ApplyEngine {
public:
    ApplyReport apply(LevelEditorLayer* layer, const std::vector<Suggestion>& suggestions, bool previewOnly);
};

} // namespace DecorationAssistant::Core
