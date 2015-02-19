
// Includes
#include <shape_comp.h>


namespace winapp {

  ShapeComp::ShapeComp(const std::vector<unsigned int> &indices, const std::vector<Vec3> &vertices, const std::vector<Vec3> &normals) :
    Component(),
    vertices_(vertices),
    normals_(normals),
    indices_(indices)
    {

    }

}
// EO Namespace