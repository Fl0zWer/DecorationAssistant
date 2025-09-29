#include "EditorScan.hpp"
#include "../integration/EditorAdapter.hpp"

using namespace geode::prelude;

namespace DecorationAssistant::Core {

ScanResult EditorScan::scan(LevelEditorLayer* layer, const CCRect& area, size_t grid) {
    ScanResult result;
    result.bounds = area;
    if (!layer) {
        log::warn("EditorScan called without layer");
        return result;
    }
    auto objects = CCArrayExt<GameObject*>(layer->getAllObjects());
    if (objects.empty()) {
        result.averageDensity = 0.f;
        return result;
    }

    float totalDensity = 0.f;
    const float cellWidth = area.size.width / static_cast<float>(grid);
    const float cellHeight = area.size.height / static_cast<float>(grid);

    for (size_t x = 0; x < grid; ++x) {
        for (size_t y = 0; y < grid; ++y) {
            CCRect cellRect(
                area.origin.x + cellWidth * x,
                area.origin.y + cellHeight * y,
                cellWidth,
                cellHeight);
            ScanCell cell;
            cell.region.bounds = cellRect;
            cell.region.label = fmt::format("cell_{}_{}", x, y);

            size_t structural = 0;
            size_t decorative = 0;
            for (auto* obj : objects) {
                if (!obj) continue;
                if (!cellRect.containsPoint(obj->getPosition())) continue;
                if (obj->m_objectType == GameObjectType::Decoration) {
                    ++decorative;
                } else {
                    ++structural;
                }
                if (obj->m_objectID == 12 /* portal */) {
                    cell.hasPortal = true;
                }
            }
            const float total = static_cast<float>(structural + decorative);
            if (total > 0.f) {
                cell.structuralDensity = structural / total;
                cell.decorativeDensity = decorative / total;
                cell.region.density = cell.decorativeDensity;
                totalDensity += cell.region.density;
            }
            result.cells.push_back(cell);
        }
    }
    if (!result.cells.empty()) {
        result.averageDensity = totalDensity / static_cast<float>(result.cells.size());
    }
    return result;
}

} // namespace DecorationAssistant::Core
