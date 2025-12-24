// include/navi/gfx/Mesh.h
#pragma once
#include <cstdint>
#include "navi/geometry/MeshData.h"
#include "navi/gfx/GLTypes.h"

namespace navi {
class Mesh {
public:
    explicit Mesh(const MeshData& data);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) noexcept;
    Mesh& operator=(Mesh&&) noexcept;

    void draw() const;

private:
    void upload(const MeshData& data);

    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
    int32_t indexCount_ = 0;
};
} // namespace navi
