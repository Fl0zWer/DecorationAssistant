#include "Telemetry.hpp"
#include <Geode/utils/file.hpp>
#include <nlohmann/json.hpp>

using namespace geode::prelude;

namespace DecorationAssistant::Core {

Telemetry& Telemetry::get() {
    static Telemetry instance;
    return instance;
}

void Telemetry::registerSession() {
    log::info("Telemetry session started");
}

void Telemetry::logScan() { m_snapshot.scans++; }
void Telemetry::logSuggestion(size_t count) { m_snapshot.suggestions += count; }
void Telemetry::logApplied(size_t count) { m_snapshot.applied += count; }
void Telemetry::logUndo() { m_snapshot.undo++; }
void Telemetry::logRedo() { m_snapshot.redo++; }

void Telemetry::flush() {
    nlohmann::json json;
    json["scans"] = m_snapshot.scans;
    json["suggestions"] = m_snapshot.suggestions;
    json["applied"] = m_snapshot.applied;
    json["undo"] = m_snapshot.undo;
    json["redo"] = m_snapshot.redo;
    file::writeString("telemetry_snapshot.json", json.dump(2));
}

} // namespace DecorationAssistant::Core
