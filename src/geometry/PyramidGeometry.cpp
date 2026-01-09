#include "navi/geometry/PyramidGeometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace navi
{

    namespace
    {

        // 삼각형 면 추가(하드 엣지: 면마다 정점 분리)
        inline void addTri(MeshData &out,
                           const glm::vec3 &a,
                           const glm::vec3 &b,
                           const glm::vec3 &c,
                           const glm::vec3 &n,
                           const glm::vec2 &uva = {0, 0},
                           const glm::vec2 &uvb = {1, 0},
                           const glm::vec2 &uvc = {0.5f, 1})
        {
            uint32_t base = static_cast<uint32_t>(out.vertices.size());
            out.vertices.push_back(Vertex{a});
            out.vertices.push_back(Vertex{b});
            out.vertices.push_back(Vertex{c});

            out.indices.push_back(base + 0);
            out.indices.push_back(base + 1);
            out.indices.push_back(base + 2);
        }

        inline glm::vec3 faceNormal(const glm::vec3 &a,
                                    const glm::vec3 &b,
                                    const glm::vec3 &c)
        {
            // CCW 기준: (b-a) x (c-a)
            glm::vec3 n = glm::cross(b - a, c - a);
            float len2 = glm::dot(n, n);
            if (len2 <= 1e-12f)
                return glm::vec3(0, 1, 0);
            return glm::normalize(n);
        }

        // 사각형(바닥) 면 추가: 4정점 + 2삼각형
        inline void addQuad(MeshData &out,
                            const glm::vec3 &p0,
                            const glm::vec3 &p1,
                            const glm::vec3 &p2,
                            const glm::vec3 &p3,
                            const glm::vec3 &n)
        {
            uint32_t base = static_cast<uint32_t>(out.vertices.size());
            out.vertices.push_back(Vertex{p0});
            out.vertices.push_back(Vertex{p1});
            out.vertices.push_back(Vertex{p2});
            out.vertices.push_back(Vertex{p3});

            out.indices.push_back(base + 0);
            out.indices.push_back(base + 1);
            out.indices.push_back(base + 2);

            out.indices.push_back(base + 0);
            out.indices.push_back(base + 2);
            out.indices.push_back(base + 3);
        }

    } // anonymous namespace

    PyramidGeometry::PyramidGeometry(float baseSize, float height, bool capBase)
        : base_(baseSize), height_(height), capBase_(capBase) {}

    MeshData PyramidGeometry::generate() const
    {
        MeshData out;

        // 대략 예상 capacity
        // side: 4 faces * 3 vertices = 12 vertices, 12 indices
        // base: +4 vertices + 6 indices
        out.vertices.reserve(capBase_ ? 16 : 12);
        out.indices.reserve(capBase_ ? 18 : 12);

        const float half = base_ * 0.5f;

        // 바닥은 y=0, 꼭짓점은 y=height_
        const glm::vec3 v0(-half, 0.0f, -half);
        const glm::vec3 v1(+half, 0.0f, -half);
        const glm::vec3 v2(+half, 0.0f, +half);
        const glm::vec3 v3(-half, 0.0f, +half);
        const glm::vec3 apex(0.0f, height_, 0.0f);

        // 옆면 4개 (CCW로 바깥 노멀 나오도록 순서 조심)
        // face 0: v0 -> v1 -> apex
        {
            glm::vec3 n = faceNormal(v0, v1, apex);
            addTri(out, v0, v1, apex, n);
        }
        // face 1: v1 -> v2 -> apex
        {
            glm::vec3 n = faceNormal(v1, v2, apex);
            addTri(out, v1, v2, apex, n);
        }
        // face 2: v2 -> v3 -> apex
        {
            glm::vec3 n = faceNormal(v2, v3, apex);
            addTri(out, v2, v3, apex, n);
        }
        // face 3: v3 -> v0 -> apex
        {
            glm::vec3 n = faceNormal(v3, v0, apex);
            addTri(out, v3, v0, apex, n);
        }

        // 바닥 캡(옵션): 바닥은 아래(-Y)를 향해야 "바깥"이 됨
        if (capBase_)
        {
            // 바닥을 -Y로 향하게 하려면 CCW를 "아래에서 봤을 때" 기준으로 잡아야 함
            // 즉 위에서 보면 CW처럼 보이는 순서가 -Y 노멀을 만듦.
            addQuad(out, v0, v3, v2, v1, glm::vec3(0, -1, 0));
        }

        return out;
    }

} // namespace navi
