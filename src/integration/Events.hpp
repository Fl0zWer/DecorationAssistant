#pragma once

#include <Geode/Geode.hpp>

namespace DecorationAssistant::Integration::Events {

struct SuggestionApplied final : public geode::Event {
    std::string label;
    size_t objectCount = 0;
};

struct UndoStackChanged final : public geode::Event {
    size_t undo = 0;
    size_t redo = 0;
    std::string lastLabel;
};

} // namespace DecorationAssistant::Integration::Events
