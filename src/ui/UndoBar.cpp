#include "UndoBar.hpp"
#include <Geode/ui/NodeIDs.hpp>
#include <fmt/format.h>

using namespace geode::prelude;
using namespace DecorationAssistant;

namespace DecorationAssistant::UI {

namespace {
class UndoListener final : public EventListener<Integration::Events::UndoStackChanged> {
protected:
    void handle(const Integration::Events::UndoStackChanged& event) override {
        if (auto scene = CCDirector::get()->getRunningScene()) {
            if (auto bar = typeinfo_cast<UndoBar*>(scene->getChildByID("da-undo-bar"))) {
                bar->updateState(event.undo, event.redo, event.lastLabel);
            }
        }
    }
};
}

UndoBar* UndoBar::create() {
    auto bar = new UndoBar();
    if (bar && bar->init()) {
        bar->autorelease();
        return bar;
    }
    CC_SAFE_DELETE(bar);
    return nullptr;
}

bool UndoBar::init() {
    if (!CCLayer::init()) {
        return false;
    }
    this->setContentSize({220.f, 48.f});
    auto bg = CCLayerColor::create({0, 0, 0, 120}, this->getContentSize().width, this->getContentSize().height);
    bg->setID(NodeIDs::fromString("da-undo-bar-bg"));
    this->addChild(bg);

    auto undoBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png"),
        nullptr,
        [](CCObject*) {
            Core::UndoManager::get().undo();
        });
    auto redoBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png"),
        nullptr,
        [](CCObject*) {
            Core::UndoManager::get().redo();
        });
    auto menu = CCMenu::create(undoBtn, redoBtn, nullptr);
    menu->alignItemsHorizontallyWithPadding(12.f);
    menu->setPosition({60.f, this->getContentSize().height / 2});
    this->addChild(menu);

    auto label = CCLabelBMFont::create("Sin acciones", "chatFont.fnt");
    label->setAnchorPoint({0.f, 0.5f});
    label->setPosition({110.f, this->getContentSize().height / 2});
    label->setID(NodeIDs::fromString("da-undo-label"));
    this->addChild(label);

    static auto listener = new UndoListener();
    listener->retain();

    return true;
}

void UndoBar::updateState(size_t undo, size_t redo, const std::string& label) {
    if (auto text = this->getChildByID("da-undo-label")) {
        if (auto bm = typeinfo_cast<CCLabelBMFont*>(text)) {
            bm->setString(fmt::format("↶ {} · ↷ {} · {}", undo, redo, label.empty() ? "Sin etiqueta" : label).c_str());
        }
    }
}

} // namespace DecorationAssistant::UI
