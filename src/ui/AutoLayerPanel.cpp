#include "AutoLayerPanel.hpp"
#include <Geode/ui/NodeIDs.hpp>
#include <Geode/loader/Mod.hpp>
#include <functional>

using namespace geode::prelude;

namespace DecorationAssistant::UI {

namespace {
CCMenuItemSpriteExtra* makeTextButton(const std::string& text, std::function<void()> callback) {
    auto label = CCLabelBMFont::create(text.c_str(), "goldFont.fnt");
    label->setScale(0.6f);
    auto sprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
    label->setPosition(sprite->getContentSize() / 2);
    sprite->addChild(label);
    return CCMenuItemSpriteExtra::create(sprite, nullptr, [callback](CCObject*) {
        if (callback) callback();
    });
}
}

AutoLayerPanel* AutoLayerPanel::create(LevelEditorLayer* editor) {
    auto layer = new AutoLayerPanel();
    if (layer && layer->init(editor)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

void AutoLayerPanel::show(LevelEditorLayer* editor) {
    auto director = CCDirector::get();
    auto scene = director->getRunningScene();
    if (!scene) return;
    if (auto existing = scene->getChildByID("da-autolayer-panel")) {
        existing->removeFromParent();
    }
    auto panel = AutoLayerPanel::create(editor);
    if (panel) {
        panel->setID(NodeIDs::fromString("da-autolayer-panel"));
        scene->addChild(panel, 550);
    }
}

bool AutoLayerPanel::init(LevelEditorLayer* editor) {
    if (!CCLayerColor::initWithColor({0, 0, 0, 210})) {
        return false;
    }
    m_editor = editor;
    this->setContentSize({400.f, 300.f});
    this->setPosition({60.f, 60.f});
    m_assigner.load("data/layer_presets.json");
    auto presetName = Mod::get()->getSettingValue<std::string>("autolayer_active_preset");
    m_preset = m_assigner.presetByName(presetName);
    buildUI();
    return true;
}

void AutoLayerPanel::buildUI() {
    auto title = CCLabelBMFont::create("\xF0\x9F\xA7\xB1 Auto Capa", "goldFont.fnt");
    title->setAnchorPoint({0.f, 1.f});
    title->setPosition({20.f, this->getContentSize().height - 20.f});
    this->addChild(title);

    auto menu = CCMenu::create();
    menu->setPosition({this->getContentSize().width - 100.f, this->getContentSize().height / 2});
    menu->addChild(makeTextButton("Simular", [this]() { this->simulate(); }));
    menu->addChild(makeTextButton("Aplicar", [this]() { this->apply(); }));
    menu->alignItemsVerticallyWithPadding(12.f);
    this->addChild(menu);
}

void AutoLayerPanel::simulate() {
    if (!m_editor || !m_preset) return;
    bool selectionOnly = Mod::get()->getSettingValue<bool>("learn_from_selection_only");
    auto plan = m_assigner.buildPlan(m_editor, *m_preset, selectionOnly);
    log::info("Simulated {} layer items", plan.items.size());
}

void AutoLayerPanel::apply() {
    if (!m_editor || !m_preset) return;
    bool selectionOnly = Mod::get()->getSettingValue<bool>("learn_from_selection_only");
    auto plan = m_assigner.buildPlan(m_editor, *m_preset, selectionOnly);
    m_assigner.apply(m_editor, plan, Mod::get()->getSettingValue<bool>("apply_preview_only"));
}

} // namespace DecorationAssistant::UI
