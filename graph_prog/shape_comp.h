#ifndef _BASE_SHAPE_H
#define _BASE_SHAPE_H

// Includes
#include <lnfw/components/component.h>
#include <vector>
#include <lnfw/physics/Vector3/Vec3.h>
#include <texture_comp.h>

namespace winapp {
  
  class ShapeComp : public lnfw::Component
  {
  public:
    // Register the class
    REGISTER_COMPONENT(ShapeComp);

    // Ctor
    ShapeComp(const std::vector<GLushort> &indices, 
              const std::vector<Vec3> &vertices, 
              const std::vector<Vec3> &normals,
              const std::vector<Texel> &texels);

    // Getters and setters
    inline const std::vector<Vec3> &getVertices() const  {
      return vertices_;
    }
    inline const std::vector<Vec3> &getNormals() const {
      return normals_;
    }
    inline const std::vector<GLushort> &getIndices() const {
      return indices_;
    }
    inline const std::vector<Texel> &getTexels() const {
      return texels_;
    }
    inline GLuint getDList() const {
      return dList_;
    }
    inline void setVertices(const std::vector<Vec3> &vect) {
      vertices_ = vect;
    }
    inline void setNormals(const std::vector<Vec3> &vect) {
      normals_ = vect;
    }
    inline void setIndices(const std::vector<GLushort> &vect) {
      indices_ = vect;
    }
    inline void setTexels(const std::vector<Texel> &vect) {
      texels_ = vect;
    }
    inline void setDList(const GLuint param) {
      dList_ = param;
    }
    
    // Create a dlist for this shape
    void createDList();

  private:
    // Vertices, normals and indices for the shape
    std::vector<Vec3> vertices_;
    std::vector<Vec3> normals_;
    std::vector<GLushort> indices_;
    // Texture coordinates
    std::vector<Texel> texels_;

    // Eventual dlist for the shape
    GLuint dList_;

  };
  // EO Class

}
// EO Namespace


#endif