
// Includes
#include <shape_comp.h>


namespace winapp {

  REGISTER_COMPONENT_IMPL(ShapeComp);

  ShapeComp::ShapeComp(const std::vector<GLubyte> &indices, 
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