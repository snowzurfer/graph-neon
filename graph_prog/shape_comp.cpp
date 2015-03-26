
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
                      const std::vector<Texel> &texels,
                      const std::vector<Face> &faces) :
    Component(), vertices_(vertices),
    normals_(normals), indices_(indices),
    texels_(texels), dList_(-1),
    faces_(faces), renderingDir_(GL_CCW)
    {
      computePlanes_();
    }

    ShapeComp::ShapeComp(const std::vector<GLushort> &indices, 
      const std::vector<Vec3> &vertices, 
      const std::vector<Vec3> &normals,
      const std::vector<Texel> &texels) :
    Component(), vertices_(vertices),
      normals_(normals), indices_(indices),
      texels_(texels), dList_(-1),
      faces_(), renderingDir_(GL_CCW)
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
        vertRenderer.render(&lnfw::Transform<Vec3>(),
          this);
      glEndList();
      
    }

    void ShapeComp::computePlanes_() {
      // For each face in the shape minus the next one which this will be 
      // checked against
      for(unsigned int i = 0; i < faces_.size(); ++i) {
        // For each one of the edges in A
        for(unsigned int edgeA = 0; edgeA < 3; ++edgeA) {
          // If the current edge doesn't already have a neighbour
          if(!faces_[i].neighIndices_[edgeA]) {
            // For each other face in the shape
            for(unsigned int j = 0; j < faces_.size(); ++j) {
              // If the face is the same (i)
              if(j == i) {
                continue;
              }

              // For each edge in B
              for(unsigned int edgeB = 0; edgeB < 3; ++edgeB) {
                // Retrieve the index of the vertices of the edges
                GLushort faceIedgeA = faces_[i].vertexIndices_[edgeA];
                GLushort faceIedgeB = faces_[i].vertexIndices_[(edgeA + 1) 
                  % 3];
                GLushort faceJedgeA = faces_[j].vertexIndices_[edgeB];
                GLushort faceJedgeB = faces_[j].vertexIndices_[(edgeB + 1) 
                  % 3];

                // If the two edges are neighbours
                if((faceIedgeA == faceJedgeA && faceIedgeB == faceJedgeB) || 
                  faceIedgeA == faceJedgeB && faceIedgeB == faceJedgeA) {
                    // Set the neighbours
                    faces_[i].neighIndices_[edgeA] = j;
                    faces_[j].neighIndices_[edgeB] = i;
                }
              }
            }
          }
        }
      }

      // Also compute the equation of the plane for each face
      for(unsigned int i = 0; i < faces_.size(); ++i) {
        faces_[i].calcPlane(vertices_);
      }
    }

    void ShapeComp::invertNormals() {
      // For each normal in the shape
      for(unsigned int i = 0; i < normals_.size(); ++i) {
        normals_[i] = normals_[i].scale(-1);
      }
    }

}
// EO Namespace