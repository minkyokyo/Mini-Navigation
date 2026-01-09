#include "navi/gfx/Mesh.h"

#include <utility> // std::exchange
#include <cstddef> // offsetof
#include <iostream>

#include "navi/geometry/Vertex.h"

namespace navi
{

    Mesh::Mesh(const MeshData &data)
    {
        upload(data);
    }

    Mesh::~Mesh()
    {
        if (ebo_)
            glDeleteBuffers(1, &ebo_);
        if (vbo_)
            glDeleteBuffers(1, &vbo_);
        if (vao_)
            glDeleteVertexArrays(1, &vao_);
    }

    Mesh::Mesh(Mesh &&o) noexcept
    {
        vao_ = std::exchange(o.vao_, 0);
        vbo_ = std::exchange(o.vbo_, 0);
        ebo_ = std::exchange(o.ebo_, 0);
        indexCount_ = std::exchange(o.indexCount_, 0);
    }

    Mesh &Mesh::operator=(Mesh &&o) noexcept
    {
        if (this == &o)
            return *this;
        // 기존 리소스 해제
        if (ebo_)
            glDeleteBuffers(1, &ebo_);
        if (vbo_)
            glDeleteBuffers(1, &vbo_);
        if (vao_)
            glDeleteVertexArrays(1, &vao_);

        vao_ = std::exchange(o.vao_, 0);
        vbo_ = std::exchange(o.vbo_, 0);
        ebo_ = std::exchange(o.ebo_, 0);
        indexCount_ = std::exchange(o.indexCount_, 0);
        return *this;
    }

    void Mesh::upload(const MeshData &data)
    {
        indexCount_ = static_cast<int32_t>(data.indices.size());

        std::cout << indexCount_ << " indices uploaded to mesh.\n";

        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER,
                     (GLsizeiptr)(data.vertices.size() * sizeof(Vertex)),
                     data.vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (GLsizeiptr)(data.indices.size() * sizeof(uint32_t)),
                     data.indices.data(),
                     GL_STATIC_DRAW);

        // layout(location=0) pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, pos));

        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void Mesh::draw() const
    {
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, (void *)0);
    }

} // namespace navi
