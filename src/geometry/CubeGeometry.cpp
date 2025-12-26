#include "navi/geometry/CubeGeometry.h"
#include "navi/geometry/BoxGeometry.h"

namespace navi {

CubeGeometry::CubeGeometry(float size) : size_(size) {}

MeshData CubeGeometry::generate() const {
    // 정육면체 = 박스의 특수 케이스
    BoxGeometry box(size_, size_, size_);
    return box.generate();
}

} // namespace navi
