#pragma once

#include <Geode/Geode.hpp>

namespace DecorationAssistant::Utils {

class ScopedTimer {
public:
    explicit ScopedTimer(const std::string& label);
    ~ScopedTimer();

private:
    std::string m_label;
    std::chrono::steady_clock::time_point m_start;
};

} // namespace DecorationAssistant::Utils
