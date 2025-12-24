// include/navi/geometry/BoxGeometry.h
#pragma once
#include "navi/geometry/IProceduralGeometry.h"

namespace navi {
class BoxGeometry final : public IProceduralGeometry {
public:
    BoxGeometry(float width, float height, float depth);
    MeshData generate() const override;

private:
    float w_, h_, d_;
};
} // namespace navi
