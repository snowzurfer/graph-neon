
// Includes
#include <shape_comp.h>
#include <vertex_renderer_comp.h>
#include <lnfw/physics/transform.h>
#include <material_comp.h>
#include <texture_comp.h>


namespace winapp {

  REGISTER_COMPONENT_IMPL(ShapeComp);

  ShapeComp::ShapeComp(const std::vector<GLuint> &indices, 
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

    ShapeComp::ShapeComp(const std::vector<GLuint> &indices, 
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
      unsigned int facesSize = faces_.size();
      for(unsigned int i = 0; i < facesSize; ++i) {
        // For each one of the edges in A
        for(unsigned int edgeA = 0; edgeA < 3; ++edgeA) {
          // If the current edge doesn't already have a neighbour
          if(faces_[i].neighIndices_[edgeA] == -1) {
            // For each other face in the shape
            for(unsigned int j = 0; j < facesSize; ++j) {
              // If the face is the same (i)
              if(j == i) {
                continue;
              }

              // Retrieve the index of the vertices of the edges
              GLuint faceIedgeA = faces_[i].vertexIndices_[edgeA];
              GLuint faceIedgeB = faces_[i].vertexIndices_[(edgeA + 1) 
                % 3];

              // Retrieve the vertices themselves
              const Vec3 &faceIvertxA = vertices_[faceIedgeA];
              const Vec3 &faceIvertxB = vertices_[faceIedgeB];

              // For each edge in B
              for(unsigned int edgeB = 0; edgeB < 3; ++edgeB) {
                GLuint faceJedgeA = faces_[j].vertexIndices_[edgeB];
                GLuint faceJedgeB = faces_[j].vertexIndices_[(edgeB + 1) 
                  % 3];

                // Retrieve the vertices themselves
                const Vec3 &faceJvertxA = vertices_[faceJedgeA];
                const Vec3 &faceJvertxB = vertices_[faceJedgeB];

                // If the two edges are neighbours
                if((faceIvertxA == faceJvertxA && faceIvertxB == faceJvertxB) || 
                  faceIvertxA == faceJvertxB && faceIvertxB == faceJvertxA) {
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