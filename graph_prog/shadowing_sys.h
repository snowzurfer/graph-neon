#ifndef _SHADOWING_SYS_H
#define _SHADOWING_SYS_H


// Includes
#include <lnfw/systems/system.h>
#include <list>
#include <light.h>
#include <shape_comp.h>
#include <vector>
#include <lnfw/physics/Vector3/Vec3.h>

namespace winapp {

  typedef float GLvector4f[4];							// Typedef's For vMatMult_ function
  typedef float GLmatrix16f[16];						// Typedef's For vMatMult_ function

  
  // Multiply a vector by a matrix, 4 by 4
  void vMat4Mult_(GLmatrix16f M, GLvector4f v);

  class ShadowingSys : public lnfw::System
  {
  public:
    // Render shadows for the entities which have a shadow component
    void update(const std::list<lnfw::Entity *> &entities);

    

  private:
    // Vector used to store temporary back of shadow volume
    std::vector<Vec3> backFacesVertices_;

    // Render shadow polys
    void doShadowPass_(const ShapeComp &shapeComp, const Light &light);
    

  };
  // EO Class

}
// EO Namespace

#endif