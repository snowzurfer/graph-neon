
// Includes
#include <shape_comp.h>
#include <vertex_renderer_comp.h>
#include <lnfw/physics/transform.h>
#include <material_comp.h>
#include <texture_comp.h>


namespace winapp {

  REGISTER_COMPONENT_IMPL(ShapeComp);

  ShapeComp::ShapeComp(const std::vector<GLushort> &indices, 
                      const std::vector<Vec3> &vertices, 
                      const std::vector<Vec3> &normals,
                      const std::vector<Texel> &texels) :
    Component(),
    vertices_(vertices),
    normals_(normals),
    indices_(indices),
    texels_(texels),
    dList_(-1)
    {

    }

    void ShapeComp::createDList() {
      // If it has already been created
      if(dList_ != -1) {
        return;
      }

      // Create a name for the dlist
      dList_ = glGenLists(1);
      // Create a renderer to render into the dlist
      VertexRendererComp vertRenderer;

      // Start compiling the list
      glNewList(dList_, GL_COMPILE);
        vertRenderer().render(&lnfw::Transform<Vec3>(),
          this);
      glEndList();
      
    }

}
// EO Namespace