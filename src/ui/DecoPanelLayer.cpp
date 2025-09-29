#include "DecoPanelLayer.hpp"
#include "OverlayHud.hpp"
#include "UndoBar.hpp"
#include <Geode/ui/NodeIDs.hpp>
#include <Geode/loader/Mod.hpp>
#include <fmt/format.h>
#include <functional>

using namespace geode::prelude;
using namespace DecorationAssistant;

namespace DecorationAssistant::UI {

DecoPanelLayer* DecoPanelLayer::create(LevelEditorLayer* editor) {
    auto layer = new DecoPanelLayer();
    if (layer && layer->init(editor)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

void DecoPanelLayer::toggle(LevelEditorLayer* editor) {
    auto director = CCDirector::get();
    auto scene = director->getRunningScene();
    if (!scene) return;
    if (auto existing = scene->getChildByID("da-deco-panel")) {
        existing->removeFromParent();
        return;
    }
    auto layer = DecoPanelLayer::create(editor);
    if (layer) {
        layer->setID(NodeIDs::fromString("da-deco-panel"));
        scene->addChild(layer, 500);
    }
}

bool DecoPanelLayer::init(LevelEditorLayer* editor) {
    if (!CCLayerColor::initWithColor({0, 0, 0, 180})) {
        return false;
    }
    m_editor = editor;
    this->setContentSize({420.f, CCDirector::get()->getWinSize().height});
    this->setPosition({20.f, 0.f});

    m_rules.loadFromFile("data/rules_minimal.json");
    m_palettes.load("data/default_palettes.json");

    buildUI();
    refreshSuggestions();
    return true;
}

void DecoPanelLayer::buildUI() {
    auto title = CCLabelBMFont::create("\xF0\x9F\x92\xA1 Asistente Deco", "goldFont.fnt");
    title->setAnchorPoint({0.f, 1.f});
    title->setPosition({20.f, this->getContentSize().height - 20.f});
    this->addChild(title);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png"),
        nullptr,
        [this](CCObject*) {
            this->removeFromParent();
        });
    auto menu = CCMenu::create();
    menu->addChild(closeBtn);
    menu->setPosition({this->getContentSize().width - 24.f, this->getContentSize().height - 24.f});
    this->addChild(menu);
}

void DecoPanelLayer::refreshSuggestions() {
    if (!m_editor) return;
    Core::Telemetry::get().logScan();
    bool selectionOnly = Mod::get()->getSettingValue<bool>("learn_from_selection_only");
    auto bounds = m_selector.selectionBounds(m_editor, selectionOnly);
    auto scan = m_scan.scan(m_editor, bounds, 6);
    Core::RuleContext ctx;
    ctx.scan = scan;
    ctx.audioDrop = m_audio.analyze(m_editor).hasDrop;
    ctx.styleHint = Mod::get()->getSettingValue<std::string>("theme_mode");
    ctx.contrastTarget = 0.7f;
    ctx.rng.seed(Mod::get()->getSettingValue<int64_t>("seed"));
    auto suggestions = m_rules.evaluate(ctx, 6);
    Core::Telemetry::get().logSuggestion(suggestions.size());
    // TODO render suggestion list
}

} // namespace DecorationAssistant::UI
