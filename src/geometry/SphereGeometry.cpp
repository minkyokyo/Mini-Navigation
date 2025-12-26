#include "navi/geometry/SphereGeometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <cmath>

namespace navi {

SphereGeometry::SphereGeometry(float diameter, int slices, int stacks)
    : diameter_(diameter),
      slices_(slices < 3 ? 3 : slices),
      stacks_(stacks < 2 ? 2 : stacks) {}

MeshData SphereGeometry::generate() const {
    MeshData out;
    const float r = diameter_ * 0.5f;

    // (stacks+1) * (slices+1) : seam(경도) 중복 포함해서 UV/인덱스 깔끔하게
    out.vertices.reserve((stacks_ + 1) * (slices_ + 1));
    out.indices.reserve(stacks_ * slices_ * 6);

    const float pi = glm::pi<float>();
    const float twoPi = glm::two_pi<float>();

    for (int i = 0; i <= stacks_; ++i) {
        float v = static_cast<float>(i) / static_cast<float>(stacks_);
        float theta = v * pi;               // 0..pi (위->아래)
        float sinT = std::sin(theta);
        float cosT = std::cos(theta);

        for (int j = 0; j <= slices_; ++j) {
            float u = static_cast<float>(j) / static_cast<float>(slices_);
            float phi = u * twoPi;          // 0..2pi
            float sinP = std::sin(phi);
            float cosP = std::cos(phi);

            glm::vec3 pos{
                r * sinT * cosP,
                r * cosT,
                r * sinT * sinP
            };

            glm::vec3 nrm = (r > 0.0f) ? glm::normalize(pos) : glm::vec3(0, 1, 0);

            Vertex vx;
            vx.pos    = pos;
            vx.normal = nrm;
            vx.uv     = glm::vec2(u, 1.0f - v); // 보통 텍스처 기준으로 v 뒤집음
            out.vertices.push_back(vx);
        }
    }

    // 인덱스: 각 패치를 2삼각형으로
    const int stride = slices_ + 1;
    for (int i = 0; i < stacks_; ++i) {
        for (int j = 0; j < slices_; ++j) {
            uint32_t a = static_cast<uint32_t>(i * stride + j);
            uint32_t b = static_cast<uint32_t>((i + 1) * stride + j);
            uint32_t c = static_cast<uint32_t>(i * stride + (j + 1));
            uint32_t d = static_cast<uint32_t>((i + 1) * stride + (j + 1));

            // CCW (바깥쪽이 앞면) 기준
            out.indices.push_back(a);
            out.indices.push_back(b);
            out.indices.push_back(c);

            out.indices.push_back(c);
            out.indices.push_back(b);
            out.indices.push_back(d);
        }
    }

    return out;
}

}
