#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "integration/Hooks.hpp"
#include "integration/Events.hpp"
#include "core/Telemetry.hpp"
#include "core/Persist.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    log::info("DecorationAssistant loaded");
    DecorationAssistant::Integration::installHooks();
    DecorationAssistant::Core::Telemetry::get().registerSession();
    DecorationAssistant::Core::Persist::get().load();
}

$on_mod(Unloaded) {
    DecorationAssistant::Core::Persist::get().flush();
    DecorationAssistant::Core::Telemetry::get().flush();
}
