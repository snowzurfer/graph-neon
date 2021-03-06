#ifndef _WINAPP_FACE_H
#define _WINAPP_FACE_H


// Includes
#include <vector>
#include <Windows.h>
#include <gl/GL.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <vector>


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
          neighIndices_(3, -1),
          planeEq_(),
          visible_(false)
        {
          
        }

        // Calculate the equation of the plane given the
        // member vertex index and the vector containing the
        // vertices
        void calcPlane(const std::vector<Vec3> &vertices); 

        std::vector<GLuint> vertexIndices_;
        std::vector<GLuint> normalIndices_;
        std::vector<GLint> neighIndices_;
        sPlaneEq planeEq_;
        bool visible_;

  private:

  };

  // Utility function.
  // Loads a vector of faces with correctly initialised faces
  // using a vector of indices as input
  std::vector<Face> buildFacesVector(const std::vector<GLuint> &indices);

}
// EO Namespace

#endif