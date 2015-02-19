#ifndef _BASE_SHAPE_H
#define _BASE_SHAPE_H

// Includes
#include <lnfw/components/component.h>
#include <vector>
#include <lnfw/physics/Vector3/Vec3.h>

namespace winapp {
  
  class ShapeComp : public lnfw::Component
  {
  public:
    // Register the class
    REGISTER_COMPONENT(ShapeComp);

    // Ctor
    ShapeComp(const std::vector<unsigned int> &indices, 
              const std::vector<Vec3> &vertices, 
              const std::vector<Vec3> &normals);

    // Getters and setters
    inline const std::vector<Vec3> &getVertices() {
      return vertices_;
    }
    inline const std::vector<Vec3> &getNormals() {
      return normals_;
    }
    inline const std::vector<unsigned int> &getIndices() {
      return indices_;
    }
    inline void setVertices(const std::vector<Vec3> &vect) {
      vertices_ = vect;
    }
    inline void setNormals(const std::vector<Vec3> &vect) {
      normals_ = vect;
    }
    inline void setIndices(const std::vector<unsigned int> &vect) {
      indices_ = vect;
    }


  private:
    // Vertices, normals and indices for the shape
    std::vector<Vec3> vertices_;
    std::vector<Vec3> normals_;
    std::vector<unsigned int> indices_;
  };
  // EO Class

}
// EO Namespace


#endif