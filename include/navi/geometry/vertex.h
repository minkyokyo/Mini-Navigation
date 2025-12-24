// include/navi/geometry/Vertex.h
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace navi {

struct Vertex {
    glm::vec3 pos;     // layout(location=0)
    glm::vec3 normal;  // layout(location=1)
    glm::vec2 uv;      // layout(location=2)  (안 쓰면 셰이더/구조에서 제거 가능)
};
}

#endif