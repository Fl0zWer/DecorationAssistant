#pragma once

#include <Geode/Geode.hpp>
#include "Telemetry.hpp"

namespace DecorationAssistant::Core {

struct PersistedState {
    std::string lastPreset;
    std::vector<std::string> recentReferences;
};

class Persist {
public:
    static Persist& get();

    void load();
    void flush();

    PersistedState& state() { return m_state; }

private:
    PersistedState m_state;
};

} // namespace DecorationAssistant::Core
