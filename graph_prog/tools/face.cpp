
// Includes
#include <tools/face.h>

namespace winapp {

  void Face::calcPlane(const std::vector<Vec3> &vertices) {
    // Create local copies of the three vertices
    Vec3 v[3];

    for (int i = 0; i < 3; ++i) {
      v[i].setX(vertices[vertexIndices_[i]].getX());
      v[i].setY(vertices[vertexIndices_[i]].getY());
      v[i].setZ(vertices[vertexIndices_[i]].getZ());
    }

    // Calculate the equation of the plane
    planeEq_.a = v[0].getY()*(v[1].getZ()-v[2].getZ()) + v[1].getY()*(v[2].getZ()-v[0].getZ()) + 
      v[2].getY()*(v[0].getZ()-v[1].getZ());
    planeEq_.b = v[0].getZ()*(v[1].getX()-v[2].getX()) + v[1].getZ()*(v[2].getX()-v[0].getX()) + 
      v[2].getZ()*(v[0].getX()-v[1].getX());
    planeEq_.c = v[0].getX()*(v[1].getY()-v[2].getY()) + v[1].getX()*(v[2].getY()-v[0].getY()) + 
      v[2].getX()*(v[0].getY()-v[1].getY());
    planeEq_.d = -( v[0].getX()*(v[1].getY()*v[2].getZ() - v[2].getY()*v[1].getZ()) +
      v[1].getX()*(v[2].getY()*v[0].getZ() - v[0].getY()*v[2].getZ()) +
      v[2].getX()*(v[0].getY()*v[1].getZ() - v[1].getY()*v[0].getZ()) );
  }
}