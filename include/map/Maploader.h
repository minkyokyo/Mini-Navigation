#pragma once

#include "map/MapData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <iostream>

using json = nlohmann::json;

namespace map
{
    struct Vertex
    {
        float x, y; // NDC
        float r, g, b, a;
        float type; // -1 road, 0 red, 1 blue, 2 green(circle)
    };

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

    static bool TryLoadTextFile(const std::filesystem::path &path, std::string &outText)
    {
        std::ifstream ifs(path);
        if (!ifs)
            return false;
        outText.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return true;
    }

    static MapData LoadMapFromJsonFile(const std::string &preferredPath)
    {
        using json = nlohmann::json;
        namespace fs = std::filesystem;

        // Try a few sensible locations (because working directory can vary by IDE).
        std::vector<fs::path> candidates;
        candidates.emplace_back(preferredPath);
        candidates.emplace_back(fs::path("assets") / "maps" / preferredPath);
        candidates.emplace_back(fs::path("assets") / "maps" / "sample_map.json");
        candidates.emplace_back(fs::path("sample_map.json"));

        std::string text;
        fs::path loaded;

        for (const auto &c : candidates)
        {
            if (TryLoadTextFile(c, text))
            {
                loaded = c;
                break;
            }
        }

#endif // MAP_MAPLOADER_H

        if (text.empty())
        {
            std::string msg = "Failed to find map JSON. Tried:\n";
            for (const auto &c : candidates)
                msg += "  - " + c.string() + "\n";
            throw std::runtime_error(msg);
        }

        std::cout << "[Map] Loaded: " << loaded.string() << "\n";

        json j = json::parse(text);

        MapData map;

        if (j.contains("nodes"))
        {
            for (auto &n : j["nodes"])
            {
                map.nodes.push_back(Node{
                    n.value("id", 0),
                    n.value("x", 0.0f),
                    n.value("y", 0.0f)});
            }
        }

        if (j.contains("edges"))
        {
            for (auto &e : j["edges"])
            {
                Edge edge;
                edge.id = e.value("id", 0);
                edge.from = e.value("from", 0);
                edge.to = e.value("to", 0);
                edge.oneway = e.value("oneway", false);
                edge.cost = e.value("cost", 1.0f);

                if (e.contains("polyline"))
                {
                    for (auto &p : e["polyline"])
                    {
                        edge.polyline.push_back({p[0].get<float>(), p[1].get<float>()});
                    }
                }
                map.edges.push_back(std::move(edge));
            }
        }

        if (j.contains("pois"))
        {
            for (auto &p : j["pois"])
            {
                Poi poi;
                poi.id = p.value("id", std::string{});
                poi.type = parsePoiType(p.value("type", "RED_SQUARE"));
                poi.x = p.value("x", 0.0f);
                poi.y = p.value("y", 0.0f);
                poi.name = p.value("name", std::string{});
                map.pois.push_back(std::move(poi));
            }
        }

        return map;
    }

}
