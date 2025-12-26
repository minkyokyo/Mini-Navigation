#include "navi/geometry/BoxGeometry.h"

#include <glm/glm.hpp>

namespace navi {

namespace {
// 면 하나(사각형) 추가: 4정점 + 6인덱스
inline void addFace(MeshData& out,
                    const glm::vec3& p0,
                    const glm::vec3& p1,
                    const glm::vec3& p2,
                    const glm::vec3& p3,
                    const glm::vec3& n,
                    const glm::vec2& uv0 = {0,0},
                    const glm::vec2& uv1 = {1,0},
                    const glm::vec2& uv2 = {1,1},
                    const glm::vec2& uv3 = {0,1})
{
    uint32_t base = static_cast<uint32_t>(out.vertices.size());

    out.vertices.push_back(Vertex{p0, n, uv0});
    out.vertices.push_back(Vertex{p1, n, uv1});
    out.vertices.push_back(Vertex{p2, n, uv2});
    out.vertices.push_back(Vertex{p3, n, uv3});

    // CCW: (0,1,2) (0,2,3)
    out.indices.push_back(base + 0);
    out.indices.push_back(base + 1);
    out.indices.push_back(base + 2);

    out.indices.push_back(base + 0);
    out.indices.push_back(base + 2);
    out.indices.push_back(base + 3);
}
} // anonymous namespace

BoxGeometry::BoxGeometry(float width, float height, float depth)
    : w_(width), h_(height), d_(depth) {}

MeshData BoxGeometry::generate() const {
    MeshData out;
    out.vertices.reserve(24);
    out.indices.reserve(36);

    const float hx = w_ * 0.5f;
    const float hy = h_ * 0.5f;
    const float hz = d_ * 0.5f;

    // 8 코너
    const glm::vec3 p000(-hx, -hy, -hz);
    const glm::vec3 p100(+hx, -hy, -hz);
    const glm::vec3 p110(+hx, +hy, -hz);
    const glm::vec3 p010(-hx, +hy, -hz);

    const glm::vec3 p001(-hx, -hy, +hz);
    const glm::vec3 p101(+hx, -hy, +hz);
    const glm::vec3 p111(+hx, +hy, +hz);
    const glm::vec3 p011(-hx, +hy, +hz);

    // +Z (front)
    addFace(out, p001, p101, p111, p011, glm::vec3(0,0,1));
    // -Z (back)
    addFace(out, p100, p000, p010, p110, glm::vec3(0,0,-1));
    // +X (right)
    addFace(out, p101, p100, p110, p111, glm::vec3(1,0,0));
    // -X (left)
    addFace(out, p000, p001, p011, p010, glm::vec3(-1,0,0));
    // +Y (top)
    addFace(out, p011, p111, p110, p010, glm::vec3(0,1,0));
    // -Y (bottom)
    addFace(out, p000, p100, p101, p001, glm::vec3(0,-1,0));

    return out;
}

} // namespace navi
