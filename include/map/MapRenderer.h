#pragma once

#include <glad/glad.h>
#include "map/MapData.h"
#include "map/Maploader.h"

namespace map
{
    static void normToNDC(float nx, float ny, float &outX, float &outY)
    {
        outX = nx * 2.0f - 1.0f;
        outY = (1.0f - ny) * 2.0f - 1.0f; // y-flip
    }

    static void colorForPoi(PointType t, float &r, float &g, float &b)
    {
        if (t == PointType::RedSquare)
        {
            r = 1;
            g = 0;
            b = 0;
            return;
        }
        if (t == PointType::BlueSquare)
        {
            r = 0;
            g = 0;
            b = 1;
            return;
        }
        // green circle
        r = 0;
        g = 1;
        b = 0;
    }

    static float typeForPoi(PointType t)
    {
        if (t == PointType::RedSquare)
            return 0.0f;
        if (t == PointType::BlueSquare)
            return 1.0f;
        return 2.0f;
    }

    class MapRenderer
    {
    public:
        MapRenderer::MapRenderer() = default;
        MapRenderer::MapRenderer();
        void init();
        void upload(const MapData &map);
        void draw(GLuint shaderProgram);

    private:
        GLuint roadVAO = 0, roadVBO = 0;
        GLuint poiVAO = 0, poiVBO = 0;
        size_t roadCount = 0;
        size_t poiCount = 0;
    };
}