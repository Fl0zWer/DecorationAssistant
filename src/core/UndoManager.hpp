#pragma once

#include <Geode/Geode.hpp>
#include "../integration/Events.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace DecorationAssistant::Core {

struct ChangeSnapshot {
    int objectID = 0;
    std::unordered_map<std::string, float> numericProps;
    std::unordered_map<std::string, int> intProps;
};

class ICommand {
public:
    std::string label;
    virtual void apply() = 0;
    virtual void revert() = 0;
    virtual ~ICommand() = default;
};

class CreateObjectsCommand : public ICommand {
public:
    LevelEditorLayer* layer = nullptr;
    std::vector<GameObject*> created;

    ~CreateObjectsCommand() override;

    void apply() override;
    void revert() override;
};

class DeleteObjectsCommand : public ICommand {
public:
    std::vector<ChangeSnapshot> snapshots;
    void apply() override {}
    void revert() override {}
};

class ModifyPropsCommand : public ICommand {
public:
    std::vector<ChangeSnapshot> before;
    std::vector<ChangeSnapshot> after;
    void apply() override {}
    void revert() override {}
};

class CompositeCommand : public ICommand {
public:
    explicit CompositeCommand(std::string lbl) { label = std::move(lbl); }
    std::vector<std::shared_ptr<ICommand>> commands;
    void apply() override;
    void revert() override;
};

class UndoManager {
public:
    static UndoManager& get();

    void push(std::shared_ptr<ICommand> command);
    void undo();
    void redo();
    void clear();

    size_t undoCount() const { return m_undoStack.size(); }
    size_t redoCount() const { return m_redoStack.size(); }

private:
    std::vector<std::shared_ptr<ICommand>> m_undoStack;
    std::vector<std::shared_ptr<ICommand>> m_redoStack;
    size_t m_limit = 32;

    void notify();
};

} // namespace DecorationAssistant::Core
