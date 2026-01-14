#pragma once

#include <glad/glad.h>

namespace map
{
    static GLuint CreateQuadVAO()
    {
        // Full-viewport quad in NDC (covers the current viewport)
        // Two triangles.
        const float verts[] = {
            -1.f,
            -1.f,
            1.f,
            -1.f,
            1.f,
            1.f,

            -1.f,
            -1.f,
            1.f,
            1.f,
            -1.f,
            1.f,
        };

        GLuint vao = 0, vbo = 0;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

        glBindVertexArray(0);

        // NOTE: vbo is intentionally not deleted here (lifetime = program lifetime)
        return vao;
    }

    static GLuint CreateBorderVAO()
    {
        // Border as line loop in NDC
        const float verts[] = {
            -1.f,
            -1.f,
            1.f,
            -1.f,
            1.f,
            1.f,
            -1.f,
            1.f,
        };

        GLuint vao = 0, vbo = 0;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

        glBindVertexArray(0);
        return vao;
    }
}