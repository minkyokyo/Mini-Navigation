// include/navi/geometry/SphereGeometry.h
#pragma once
#include "navi/geometry/IProceduralGeometry.h"

namespace navi {
class SphereGeometry final : public IProceduralGeometry {
public:
    SphereGeometry(float diameter, int slices = 32, int stacks = 16);

    MeshData generate() const override;

private:
    float diameter_;
    int slices_;
    int stacks_;
};
} 
