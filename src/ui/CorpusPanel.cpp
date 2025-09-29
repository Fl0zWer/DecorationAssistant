#include "CorpusPanel.hpp"
#include <fmt/format.h>

using namespace geode::prelude;
using namespace DecorationAssistant;

namespace DecorationAssistant::UI {

CorpusPanel* CorpusPanel::create() {
    auto layer = new CorpusPanel();
    if (layer && layer->init()) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool CorpusPanel::init() {
    if (!CCLayer::init()) {
        return false;
    }
    this->setContentSize({400.f, 220.f});
    refresh();
    return true;
}

void CorpusPanel::refresh() {
    this->removeAllChildren();
    auto list = Core::LevelCorpus::get().list();
    float y = this->getContentSize().height - 20.f;
    for (auto& level : list) {
        auto label = CCLabelBMFont::create(fmt::format("{} ({:.2f})", level.name, level.difficulty).c_str(), "chatFont.fnt");
        label->setAnchorPoint({0.f, 1.f});
        label->setPosition({20.f, y});
        this->addChild(label);
        y -= 24.f;
    }
}

} // namespace DecorationAssistant::UI
