#include "map/MapData.h"
#include "map/Maploader.h"
#include <glad/glad.h>
#include <vector>
#include <cmath>

namespace map
{
    static void normToNDC(float nx, float ny, float &outX, float &outY)
    {
        outX = nx * 2.0f - 1.0f;
        outY = (1.0f - ny) * 2.0f - 1.0f; // y-flip
    }

    static void colorForPoi(PoiType t, float &r, float &g, float &b)
    {
        if (t == PoiType::RedSquare)
        {
            r = 1;
            g = 0;
            b = 0;
            return;
        }
        if (t == PoiType::BlueSquare)
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

    static float typeForPoi(PoiType t)
    {
        if (t == PoiType::RedSquare)
            return 0.0f;
        if (t == PoiType::BlueSquare)
            return 1.0f;
        return 2.0f;
    }

    class MapRenderer
    {
    public:
        void init()
        {
            glGenVertexArrays(1, &roadVAO);
            glGenBuffers(1, &roadVBO);

            glGenVertexArrays(1, &poiVAO);
            glGenBuffers(1, &poiVBO);
        }

        void upload(const MapData &map)
        {
            std::vector<Vertex> roadVerts;
            std::vector<Vertex> poiVerts;

            // roads: polyline -> segments
            for (const auto &e : map.edges)
            {
                if (e.polyline.size() < 2)
                    continue;
                for (size_t i = 0; i + 1 < e.polyline.size(); ++i)
                {
                    float x0, y0, x1, y1;
                    normToNDC(e.polyline[i].first, e.polyline[i].second, x0, y0);
                    normToNDC(e.polyline[i + 1].first, e.polyline[i + 1].second, x1, y1);

                    // white road (you can do gray outline later with 2-pass)
                    roadVerts.push_back({x0, y0, 1, 1, 1, 1, -1});
                    roadVerts.push_back({x1, y1, 1, 1, 1, 1, -1});
                }
            }

            // POIs as points
            for (const auto &p : map.pois)
            {
                float x, y;
                normToNDC(p.x, p.y, x, y);
                float r, g, b;
                colorForPoi(p.type, r, g, b);
                float type = typeForPoi(p.type);
                poiVerts.push_back({x, y, r, g, b, 1, type});
            }

            // upload roads
            roadCount = roadVerts.size();
            glBindVertexArray(roadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
            glBufferData(GL_ARRAY_BUFFER, roadVerts.size() * sizeof(Vertex), roadVerts.data(), GL_STATIC_DRAW);

            // layout: pos(2), color(4), type(1)
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(float)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));

            // upload pois
            poiCount = poiVerts.size();
            glBindVertexArray(poiVAO);
            glBindBuffer(GL_ARRAY_BUFFER, poiVBO);
            glBufferData(GL_ARRAY_BUFFER, poiVerts.size() * sizeof(Vertex), poiVerts.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(float)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));

            glBindVertexArray(0);
        }

        void draw(GLuint shaderProgram)
        {
            glUseProgram(shaderProgram);

            // roads
            glUniform1i(glGetUniformLocation(shaderProgram, "uMode"), 0);
            glBindVertexArray(roadVAO);
            glDrawArrays(GL_LINES, 0, (GLsizei)roadCount);

            // pois
            glEnable(GL_PROGRAM_POINT_SIZE);
            glUniform1i(glGetUniformLocation(shaderProgram, "uMode"), 1);
            glUniform1f(glGetUniformLocation(shaderProgram, "uPointSize"), 22.0f);
            glBindVertexArray(poiVAO);
            glDrawArrays(GL_POINTS, 0, (GLsizei)poiCount);

            glBindVertexArray(0);
        }

    private:
        GLuint roadVAO = 0, roadVBO = 0;
        GLuint poiVAO = 0, poiVBO = 0;
        size_t roadCount = 0;
        size_t poiCount = 0;
    };
}