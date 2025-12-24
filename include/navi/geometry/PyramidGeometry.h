// include/navi/geometry/PyramidGeometry.h
#pragma once
#include "navi/geometry/IProceduralGeometry.h"

namespace navi {
class PyramidGeometry final : public IProceduralGeometry {
public:
    PyramidGeometry(float baseSize, float height, bool capBase = true);
    MeshData generate() const override;

private:
    float base_;
    float height_;
    bool capBase_;
};
} // namespace navi
