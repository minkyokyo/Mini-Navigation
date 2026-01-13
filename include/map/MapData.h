#pragma once
#include <string>
#include <vector>
namespace map
{
    struct Node
    {
        int id;
        float x, y; // normalized 0..1
    };

    struct Edge
    {
        int id;
        int from, to;
        bool oneway = false;
        float cost = 1.0f;
        std::vector<std::pair<float, float>> polyline; // normalized points
    };

    enum class PoiType
    {
        RedSquare,
        BlueSquare,
        GreenCircle
    };

    struct Poi
    {
        std::string id;
        PoiType type;
        float x, y; // normalized
        std::string name;
    };

    struct MapData
    {
        std::vector<Node> nodes;
        std::vector<Edge> edges;
        std::vector<Poi> pois;
    };
}