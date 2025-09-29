#include "UndoManager.hpp"

using namespace geode::prelude;

namespace DecorationAssistant::Core {

void CompositeCommand::apply() {
    for (auto& cmd : commands) {
        if (cmd) cmd->apply();
    }
}

void CompositeCommand::revert() {
    for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
        if (*it) (*it)->revert();
    }
}

UndoManager& UndoManager::get() {
    static UndoManager instance;
    return instance;
}

void UndoManager::push(std::shared_ptr<ICommand> command) {
    if (!command) return;
    m_undoStack.push_back(std::move(command));
    m_redoStack.clear();
    if (m_undoStack.size() > m_limit) {
        m_undoStack.erase(m_undoStack.begin());
    }
    notify();
}

void UndoManager::undo() {
    if (m_undoStack.empty()) {
        return;
    }
    auto cmd = m_undoStack.back();
    m_undoStack.pop_back();
    if (cmd) {
        cmd->revert();
        m_redoStack.push_back(cmd);
    }
    notify();
}

void UndoManager::redo() {
    if (m_redoStack.empty()) {
        return;
    }
    auto cmd = m_redoStack.back();
    m_redoStack.pop_back();
    if (cmd) {
        cmd->apply();
        m_undoStack.push_back(cmd);
    }
    notify();
}

void UndoManager::clear() {
    m_undoStack.clear();
    m_redoStack.clear();
    notify();
}

void UndoManager::notify() {
    Integration::Events::UndoStackChanged evt;
    evt.undo = m_undoStack.size();
    evt.redo = m_redoStack.size();
    if (!m_undoStack.empty()) {
        evt.lastLabel = m_undoStack.back()->label;
    }
    evt.post();
}

} // namespace DecorationAssistant::Core
