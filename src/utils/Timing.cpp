#include "Timing.hpp"

namespace DecorationAssistant::Utils {

ScopedTimer::ScopedTimer(const std::string& label)
    : m_label(label), m_start(std::chrono::steady_clock::now()) {}

ScopedTimer::~ScopedTimer() {
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
    geode::log::debug("{} took {} ms", m_label, ms);
}

} // namespace DecorationAssistant::Utils
