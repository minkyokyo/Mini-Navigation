// include/navi/geometry/IProceduralGeometry.h
#ifndef IPROCEDURAL_GEOMETRY_H
#define IPROCEDURAL_GEOMETRY_H

#include "navi/geometry/MeshData.h"

namespace navi {
class IProceduralGeometry {
public:
    virtual ~IProceduralGeometry() = default;
    virtual MeshData generate() const = 0; // CPU에서 vertices/indices 생성
};
} // namespace navi
#endif