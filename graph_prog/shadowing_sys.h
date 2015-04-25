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

  typedef float GLvector4f[4];
  typedef float GLmatrix16f[16];

  
  // Multiply a vector by a matrix, 4 by 4
  void vMat4Mult_(GLmatrix16f M, GLvector4f v);

  // Generate a shadow matrix to cast a cap for the shadow volume
  void generateShadowMatrix_(GLmatrix16f matrix, GLvector4f light_pos, 
    const Vec3 &normal, const Vec3 &planePt);

  class ShadowingSys : public lnfw::System
  {
  public:
    // Ctor
    ShadowingSys(std::vector<Light *> &lights);


    // Dtor
    ~ShadowingSys();
  
    // Render shadows for the entities which have a shadow component
    void update(const std::list<lnfw::Entity *> &entities);

    

  private:
    // Vector used to store temporary back of shadow volume
    std::vector<float> backFacesVertices_;
    std::vector<GLuint> backFacesIndices_;
    // Vector used to store temporary front of shadow volume
    std::vector<float> frontFacesVertices_;
    std::vector<GLuint> frontFacesIndices_;

    // Render shadow polys
    void doShadowPass_(const ShapeComp &shapeComp, const Light &light);
    
    // Lights in the scene
    std::vector<Light *> &lights_;
  };
  // EO Class

}
// EO Namespace

#endif