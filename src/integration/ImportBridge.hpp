#pragma once

#include <Geode/Geode.hpp>
#include <optional>
#include <filesystem>

namespace DecorationAssistant::Integration {

struct ImportedLevel {
    std::string id;
    std::string name;
    double bpm = 0.0;
    std::vector<std::string> tags;
    CCArrayExt<GameObject*> objects;
};

class ImportBridge {
public:
    static ImportBridge& get();

    std::optional<ImportedLevel> importFromFile(const std::filesystem::path& path);
    std::optional<ImportedLevel> importFromClipboard() const;

private:
    ImportBridge() = default;
};

} // namespace DecorationAssistant::Integration
