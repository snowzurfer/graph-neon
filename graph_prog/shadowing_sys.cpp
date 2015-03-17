
// Includes
#include <shadowing_sys.h>
#include <shadow_comp.h>
#include <lnfw/physics/transform.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <tools/crc.h>
#include <base_renderer_comp.h>
#include <tools/abertay_framework.h>
#include <tools/face.h>

#define PROJ_INFINITY 100.f

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
        const lnfw::Transform<Vec3> &transform = (*entityitor)->transform;
         
        // For each light in the scene
        for(unsigned int lightNum = 0; 
          lightNum < shadowComp->getLights().size(); ++lightNum) {
          // Retrieve the light
          const Light &light = *shadowComp->getLights()[lightNum];

          // Work variables
          GLmatrix16f Minv;
          GLvector4f wlp, lp;

          // Compute the position with respect to the entity's local
          // coordinates system
          glMatrixMode(GL_MODELVIEW);
          glPushMatrix();
          glLoadIdentity();
          // Apply transformations in inverse order
          glScalef(-transform.scale.getX(), -transform.scale.getY(),
            -transform.rotation.getZ());
          glRotatef(-transform.rotation.getX(), 1.f, 0.f, 0.f);
          glRotatef(-transform.rotation.getY(), 0.f, 1.f, 0.f);
          glRotatef(-transform.rotation.getZ(), 0.f, 0.f, 1.f);
          glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix From Minv
          lp[0] = light.getPosition()[0];								// Store Light Position X In lp[0]
          lp[1] = light.getPosition()[1];								// Store Light Position Y In lp[1]
          lp[2] = light.getPosition()[2];								// Store Light Position Z In lp[2]
          lp[3] = light.getPosition()[3];								// Store Light Direction In lp[3]
          vMat4Mult_(Minv, lp);									// We Store Rotated Light Vector In 'lp' Array
          glTranslatef(-transform.position.getX(),
            -transform.position.getY(),
            -transform.position.getZ());
          glPopMatrix();
          glGetFloatv(GL_MODELVIEW_MATRIX, Minv);				// Retrieve ModelView Matrix From Minv
          wlp[0] = 0.0f;										// World Local Coord X To 0
          wlp[1] = 0.0f;										// World Local Coord Y To 0
          wlp[2] = 0.0f;										// World Local Coord Z To 0
          wlp[3] = 1.0f;
          vMat4Mult_(Minv, wlp);								// We Store The Position Of The World Origin Relative To The
          // Local Coord. System In 'wlp' Array
          lp[0] += wlp[0];									// Adding These Two Gives Us The
          lp[1] += wlp[1];									// Position Of The Light Relative To
          lp[2] += wlp[2];									// The Local Coordinate System

          Light workLight(0);
          workLight.setPosition(lp);

          // TODO move all this into a function named "computeVisibility"
          // Determine if a face is facing the workLight
          unsigned int facesNum = shapeComp->getFaces().size();
          for(unsigned int faceNum = 0; faceNum < facesNum; ++faceNum) {
            // Retrieve the plane equation of the face
            const sPlaneEq &plane = shapeComp->getFaces()[faceNum].planeEq_;

            // Calculate which surfaces are facing the workLight by
            // substituting the workLight's position into the plane
            // equation
            float side = plane.a * workLight.getPosition()[0] +
              plane.b * workLight.getPosition()[1] +
              plane.c * workLight.getPosition()[2] +
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
          doShadowPass_(*shapeComp, workLight);

          // Second pass. Decrease the stencil values where there are
          // shadows


          

          
        }
      }
    }

    // Enable rendering to color buffer and reset face rendering
    glFrontFace(GL_CCW);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Draw a shadowing rectangle covering the entire screen.
    // This rectangle will be drawn only in the areas where the stencil
    // buffer is set to 1
    glColor4f(0.f, 0.f, 0.f, 0.4f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFFL);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glPushMatrix();
    glLoadIdentity();
    glBegin( GL_TRIANGLE_STRIP );
      glVertex3f(-0.1f, 0.1f,-0.10f);
      glVertex3f(-0.1f,-0.1f,-0.10f);
      glVertex3f( 0.1f, 0.1f,-0.10f);
      glVertex3f( 0.1f,-0.1f,-0.10f);
    glEnd();
    glPopMatrix();

    // Pop the attributes set at the beginning of the function
    glPopAttrib();
  }

  void doShadowPass_(const ShapeComp &shapeComp, const Light &light) {
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
            vC.setX((vA.getX() - light.getPosition()[0]) * PROJ_INFINITY);
            vC.setY((vA.getY() - light.getPosition()[1]) * PROJ_INFINITY);
            vC.setZ((vA.getZ() - light.getPosition()[2]) * PROJ_INFINITY);

            vD.setX((vB.getX() - light.getPosition()[0]) * PROJ_INFINITY);
            vD.setY((vB.getY() - light.getPosition()[1]) * PROJ_INFINITY);
            vD.setZ((vB.getZ() - light.getPosition()[2]) * PROJ_INFINITY);

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

  void vMat4Mult_(GLmatrix16f M, GLvector4f v) {
    GLfloat res[4];										// Hold Calculated Results
    res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
    res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
    res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
    res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
    v[0]=res[0];										// Results Are Stored Back In v[]
    v[1]=res[1];
    v[2]=res[2];
    v[3]=res[3];										// Homogenous Coordinate
  }


}
// EO Namespace