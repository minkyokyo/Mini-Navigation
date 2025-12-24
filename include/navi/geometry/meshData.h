#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <vector>
#include <cstdint>
#include "navi/geometry/Vertex.h"

namespace navi {
    
struct MeshData {
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices; // ES까지 고려하면 필요시 uint16_t로 다운캐스트 가능
};
}

#endif