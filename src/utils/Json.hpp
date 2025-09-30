#pragma once

#include <Geode/Geode.hpp>
#include <nlohmann/json.hpp>
#include <optional>

namespace DecorationAssistant::Utils {

std::optional<nlohmann::json> jsonFromString(const std::string& data);

} // namespace DecorationAssistant::Utils
