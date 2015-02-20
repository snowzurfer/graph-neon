
// Includes
#include <shape_comp.h>


namespace winapp {

  ShapeComp::ShapeComp(const std::vector<unsigned int> &indices, 
                      const std::vector<Vec3> &vertices, 
                      const std::vector<Vec3> &normals,
                      const std::vector<Texel> &texels) :
    Component(),
    vertices_(vertices),
    normals_(normals),
    indices_(indices),
    texels_(texels)
    {

    }
}
// EO Namespace