
// Includes
#include <shadowing_sys.h>
#include <shadow_comp.h>
#include <lnfw/physics/transform.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <tools/crc.h>
#include <base_renderer_comp.h>
#include <tools/abertay_framework.h>
#include <tools/face.h>

#define PROJ_INFINITY 100;

namespace winapp {

  void ShadowingSys::update(const std::list<lnfw::Entity *> &entities) {
    // Iterate through the entities and render them
    for(constEntitiesItor_ entityitor = entities.begin(); entityitor != entities.end(); ++entityitor) {
      // If the entity has a renderer component and a shadow component
      if((*entityitor)->hasComp(abfw::CRC::GetICRC("BaseRendererComp")) && 
         (*entityitor)->hasComp(abfw::CRC::GetICRC("ShadowComp"))) { 
        // Retrieve the necessary components
        ShapeComp *shapeComp = (ShapeComp *)(*entityitor)->getComp(abfw::CRC::GetICRC("ShapeComp"));
        ShadowComp *shadowComp = (ShadowComp *)(*entityitor)->getComp(abfw::CRC::GetICRC("ShadowComp"));
       
         
        // For each light in the scene
        for(unsigned int lightNum = 0; 
          lightNum < shadowComp->getLights().size(); ++lightNum) {
          // Retrieve the light
          const Light &light = *shadowComp->getLights()[lightNum];
          
          // TODO move all this into a function named "computeVisibility"
          // Determine if a face is facing the light
          unsigned int facesNum = shapeComp->getFaces().size();
          for(unsigned int faceNum = 0; faceNum < facesNum; ++faceNum) {
            // Retrieve the plane equation of the face
            const sPlaneEq &plane = shapeComp->getFaces()[faceNum].planeEq_;

            // Calculate which surfaces are facing the light by
            // substituting the light's position into the plane
            // equation
            float side = plane.a * light.getPosition()[0] +
              plane.b * light.getPosition()[1] +
              plane.c * light.getPosition()[2] +
              plane.d;

            if(side > 0.f) {
              // The face is visible
              shapeComp->setFaceVisibility(faceNum, true);
            }
            else {
              // The face is not visible (it is shaded)
              shapeComp->setFaceVisibility(faceNum, false);
            }
          }

          // Push the attributes to easily retrieve them after the passes
          glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | 
            GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );
          // Turn off lighting
          glDisable(GL_LIGHTING);
          // Disable writing to the depth buffer
          glDepthMask(GL_FALSE);
          // Set the depth function to be used
          glDepthFunc(GL_LEQUAL);
          // Disable writing to the colour buffer
          glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
          // Setup the stencil buffer
          glEnable(GL_STENCIL_TEST);
          glStencilFunc(GL_ALWAYS, 1, 1);

          // First pass. Increase the stencil values where there are
          // shadows
          glFrontFace(GL_CCW);
          glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
          doShadowPass_(*shapeComp, light);

          // Second pass. Decrease the stencil values where there are
          // shadows

        }
      }
    }
  }

  void ShadowingSys::doShadowPass_(const ShapeComp &shapeComp, const Light &light) {
    // For each face in the shape
    for(int faceNum = 0; faceNum < shapeComp.getFaces().size(); ++faceNum) {
      // Retrieve the current face
      const Face &face = shapeComp.getFaces()[faceNum];

      // If the current face is visible
      if(face.visible_) {
        // For each edge of the face
        for(int e = 0; e < 3; ++e) {
          // Retrieve eventual neighbour
          int neighbourIndex = face.neighIndices_[e];

          // If there isn't a neighbour, or the neighbour isn't visible
          if((!neighbourIndex) || 
            shapeComp.getFaces()[neighbourIndex].visible_ == false) 
          {
            // Get the vertices of the edge
            const Vec3 &vA = shapeComp.getVertices()[face.vertexIndices_[e]];
            const Vec3 &vB = 
              shapeComp.getVertices()[face.vertexIndices_[(e + 1) % 3]];

            // Create the two vertices in the distance
            Vec3 vC, vD;

            // Calculate the projections
            vC.setX(vA.getX() - light.getPosition()[0]) * PROJ_INFINITY;
            vC.setY(vA.getY() - light.getPosition()[1]) * PROJ_INFINITY;
            vC.setZ(vA.getZ() - light.getPosition()[2]) * PROJ_INFINITY;

            vD.setX(vB.getX() - light.getPosition()[0]) * PROJ_INFINITY;
            vD.setY(vB.getY() - light.getPosition()[1]) * PROJ_INFINITY;
            vD.setZ(vB.getZ() - light.getPosition()[2]) * PROJ_INFINITY;

            // Render the shadows as a quadrilateral 
            // (as a triangle strip poly)
            glBegin(GL_TRIANGLE_STRIP);
              glVertex3f(vA.getX(), vA.getY(), vA.getZ());
              glVertex3f(vA.getX() + vC.getX(), vA.getY() + vC.getY(),
                vA.getZ() + vC.getZ());
              glVertex3f(vB.getX(), vB.getY(), vB.getZ());
              glVertex3f(vB.getX() + vD.getX(), vB.getY() + vD.getY(),
                vB.getZ() + vD.getZ());
            glEnd();
          }
        } 
      }
    }
  }


}
// EO Namespace