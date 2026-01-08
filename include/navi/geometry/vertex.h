// include/navi/geometry/Vertex.h
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace navi
{

    struct Vertex
    {
        glm::vec3 pos; // layout(location=0)
        // glm::vec3 normal;  // layout(location=1)
    };
}

#endif