#pragma once

#include <Geode/Geode.hpp>
#include "../core/UndoManager.hpp"
#include <string>

namespace DecorationAssistant::UI {

class UndoBar : public CCLayer {
public:
    static UndoBar* create();
    bool init();

    void updateState(size_t undo, size_t redo, const std::string& label);
};

} // namespace DecorationAssistant::UI
