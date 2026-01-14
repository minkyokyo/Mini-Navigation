#pragma once

#include "map/MapData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace map
{
    struct Vertex
    {
        float x, y; // NDC
        float r, g, b, a;
        float type; // -1 road, 0 red, 1 blue, 2 green(circle)
    };

    static PointType parsePointType(const std::string &s)
    {
        if (s == "RED_SQUARE")
            return PointType::RedSquare;
        if (s == "BLUE_SQUARE")
            return PointType::BlueSquare;
        if (s == "GREEN_CIRCLE")
            return PointType::GreenCircle;
        throw std::runtime_error("Unknown POI type: " + s);
    }

    static bool TryLoadTextFile(const std::filesystem::path &path, std::string &outText)
    {
        std::ifstream ifs(path);
        if (!ifs)
            return false;

        // 파일 열어서 통째로 outText에 넣기
        outText.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return true;
    }

    static MapData LoadMapFromJsonFile(const std::string &path)
    {

        std::string text;
        fs::path loaded;

        if (!TryLoadTextFile(path, text))
        {
            throw std::runtime_error("Failed to open map file: " + path);
        }

        if (text.empty())
        {
            throw std::runtime_error("Failed to read map Json");
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
                Point poi;
                poi.id = p.value("id", std::string{});
                poi.type = parsePointType(p.value("type", "RED_SQUARE"));
                poi.x = p.value("x", 0.0f);
                poi.y = p.value("y", 0.0f);
                poi.name = p.value("name", std::string{});
                map.pois.push_back(std::move(poi));
            }
        }

        return map;
    }

}
