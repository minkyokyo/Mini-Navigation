// include/navi/geometry/CubeGeometry.h
#pragma once
#include "navi/geometry/IProceduralGeometry.h"

namespace navi {
class CubeGeometry final : public IProceduralGeometry {
public:
    explicit CubeGeometry(float size);
    MeshData generate() const override;

private:
    float size_;
};
} 
