#pragma once

#include <Geode/Geode.hpp>

namespace DecorationAssistant::Core {

struct TelemetrySnapshot {
    uint64_t scans = 0;
    uint64_t suggestions = 0;
    uint64_t applied = 0;
    uint64_t undo = 0;
    uint64_t redo = 0;
};

class Telemetry {
public:
    static Telemetry& get();

    void registerSession();
    void logScan();
    void logSuggestion(size_t count);
    void logApplied(size_t count);
    void logUndo();
    void logRedo();
    void flush();

    const TelemetrySnapshot& snapshot() const { return m_snapshot; }

private:
    TelemetrySnapshot m_snapshot;
};

} // namespace DecorationAssistant::Core
