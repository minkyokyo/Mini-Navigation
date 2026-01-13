#include "MapData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

namespace map
{
    static PoiType parsePoiType(const std::string &s)
    {
        if (s == "RED_SQUARE")
            return PoiType::RedSquare;
        if (s == "BLUE_SQUARE")
            return PoiType::BlueSquare;
        if (s == "GREEN_CIRCLE")
            return PoiType::GreenCircle;
        throw std::runtime_error("Unknown POI type: " + s);
    }

    MapData LoadMapFromJsonFile(const std::string &path)
    {
        std::ifstream ifs(path);
        if (!ifs)
            throw std::runtime_error("Failed to open map json: " + path);

        json j;
        ifs >> j;

        MapData map;

        for (auto &n : j["nodes"])
        {
            map.nodes.push_back(Node{
                n["id"].get<int>(),
                n["x"].get<float>(),
                n["y"].get<float>()});
        }

        for (auto &e : j["edges"])
        {
            Edge edge;
            edge.id = e["id"].get<int>();
            edge.from = e["from"].get<int>();
            edge.to = e["to"].get<int>();
            edge.oneway = e.value("oneway", false);
            edge.cost = e.value("cost", 1.0f);

            for (auto &p : e["polyline"])
            {
                edge.polyline.push_back({p[0].get<float>(), p[1].get<float>()});
            }
            map.edges.push_back(std::move(edge));
        }

        for (auto &p : j["pois"])
        {
            Poi poi;
            poi.id = p["id"].get<std::string>();
            poi.type = parsePoiType(p["type"].get<std::string>());
            poi.x = p["x"].get<float>();
            poi.y = p["y"].get<float>();
            poi.name = p.value("name", "");
            map.pois.push_back(std::move(poi));
        }

        return map;
    }
}