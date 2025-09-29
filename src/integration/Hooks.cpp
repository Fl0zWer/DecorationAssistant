#include "Hooks.hpp"
#include "EditorAdapter.hpp"
#include "../ui/DecoPanelLayer.hpp"
#include "../ui/AutoLayerPanel.hpp"
#include "../ui/UndoBar.hpp"
#include "Events.hpp"
#include <Geode/ui/NodeIDs.hpp>
#include <Geode/loader/Mod.hpp>
#include <functional>

using namespace geode::prelude;

namespace DecorationAssistant::Integration {

namespace {
constexpr const char* kAssistantButtonID = "da-assistant-button";
constexpr const char* kAutoLayerButtonID = "da-autolayer-button";
constexpr const char* kUndoBarID = "da-undo-bar";

CCMenuItemSpriteExtra* makeButton(const char* label, const char* nodeID, std::function<void()> callback) {
    auto sprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
    auto title = CCLabelBMFont::create(label, "bigFont.fnt");
    title->setScale(0.45f);
    title->setPosition(sprite->getContentSize() / 2);
    sprite->addChild(title);

    auto button = CCMenuItemSpriteExtra::create(sprite, nullptr, [callback](CCObject*) {
        if (callback) callback();
    });
    button->setID(NodeIDs::fromString(nodeID));
    return button;
}

void attachUndoBar(CCLayer* parent) {
    if (!parent || parent->getChildByID(kUndoBarID)) {
        return;
    }
    auto bar = DecorationAssistant::UI::UndoBar::create();
    if (!bar) {
        return;
    }
    bar->setID(NodeIDs::fromString(kUndoBarID));
    parent->addChild(bar, 200);
    bar->setPosition({parent->getContentSize().width - 120.f, 40.f});
}

} // namespace

void installHooks() {
    log::info("Installing DecorationAssistant hooks");
    (void) EditorLayerHook::create;
}

bool EditorLayerHook::init(LevelEditorLayer* editor) {
    if (!EditorPauseLayer::init(editor)) {
        return false;
    }

    auto menu = this->getChildByIDRecursive("pause-menu");
    if (auto buttonMenu = typeinfo_cast<CCMenu*>(menu)) {
        buttonMenu->alignItemsHorizontallyWithPadding(6.f);
        auto decoBtn = makeButton("\xF0\x9F\x92\xA1", kAssistantButtonID, [editor]() {
            DecorationAssistant::UI::DecoPanelLayer::toggle(editor);
        });
        buttonMenu->addChild(decoBtn);

        auto layerBtn = makeButton("\xF0\x9F\xA7\xB1", kAutoLayerButtonID, [editor]() {
            DecorationAssistant::UI::AutoLayerPanel::show(editor);
        });
        buttonMenu->addChild(layerBtn);
    }

    attachUndoBar(this);
    return true;
}

} // namespace DecorationAssistant::Integration
