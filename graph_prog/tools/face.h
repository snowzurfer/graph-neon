#ifndef _WINAPP_FACE_H
#define _WINAPP_FACE_H


// Includes
#include <vector>
#include <Windows.h>
#include <gl/GL.h>
#include <lnfw/physics/Vector3/Vec3.h>


namespace winapp {

  struct sPlaneEq {
    float a, b, c, d;
  };

  class Face
  {
  public:
    // Ctor
    Face() :
        vertexIndices_(),
          normalIndices_(),
          neighIndices_(),
          planeEq_(),
          visible_(false)
        {
          neighIndices_.reserve(3);
          for(int i = 0; i < 3; ++i) {
            neighIndices_.push_back(0);
          }
        }

        // Calculate the equation of the plane given the
        // member vertex index and the vector containing the
        // vertices
        void calcPlane(const std::vector<Vec3> &vertices); 

        std::vector<GLushort> vertexIndices_;
        std::vector<GLushort> normalIndices_;
        std::vector<GLushort> neighIndices_;
        sPlaneEq planeEq_;
        bool visible_;

  private:

  };

}
// EO Namespace

#endif