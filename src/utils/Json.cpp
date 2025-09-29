#include "Json.hpp"

namespace DecorationAssistant::Utils {

std::optional<nlohmann::json> jsonFromString(const std::string& data) {
    auto json = nlohmann::json::parse(data, nullptr, false);
    if (json.is_discarded()) {
        return std::nullopt;
    }
    return json;
}

} // namespace DecorationAssistant::Utils
