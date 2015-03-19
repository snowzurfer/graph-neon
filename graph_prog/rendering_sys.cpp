
// Includes
#include <rendering_sys.h>
#include <lnfw/physics/transform.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <tools/crc.h>
#include <base_renderer_comp.h>
#include <tools/abertay_framework.h>
#include <shadow_comp.h>
#include <shadowing_sys.h>
#include <light.h>

namespace winapp {

  void RenderingSys::update(const std::list<lnfw::Entity *> &entities) {
    // Iterate through the entities and render them
    for(constEntitiesItor_ entityitor = entities.begin(); entityitor != entities.end(); ++entityitor) {
      // If the entity has a renderer component
      if((*entityitor)->hasComp(abfw::CRC::GetICRC("BaseRendererComp"))) { 
        // Retrieve the renderer component
        BaseRendererComp *rendererComp = (BaseRendererComp *)(*entityitor)->
          getComp(abfw::CRC::GetICRC("BaseRendererComp"));

        // Retrieve the other necessary components
        TextureComp *textureComp = (TextureComp *)(*entityitor)->getComp(abfw::CRC::GetICRC("TextureComp"));
        MaterialComp *materialComp = (MaterialComp *)(*entityitor)->getComp(abfw::CRC::GetICRC("MaterialComp"));
        ShapeComp *shapeComp = (ShapeComp *)(*entityitor)->getComp(abfw::CRC::GetICRC("ShapeComp"));

        // Push the modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

          // Render the object
          rendererComp->render(&(*entityitor)->transform,
                                shapeComp,
                                textureComp,
                                materialComp);

          // If the entity has a shadow component
          if((*entityitor)->hasComp(abfw::CRC::GetICRC("ShadowComp"))) {
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
                /*glScalef(-transform.scale.getX(), -transform.scale.getY(),
                -transform.scale.getZ());*/
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
                glPopMatrix();
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
                /*glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | 
                  GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );*/
                // Turn off lighting
                glDisable(GL_LIGHTING);
                // Disable writing to the depth buffer
                glDepthMask(GL_FALSE);
                // Set the depth function to be used
                glDepthFunc(GL_LEQUAL);
                // Setup the stencil buffer
                glEnable(GL_STENCIL_TEST);
                // Disable writing to the colour buffer
                glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                glStencilFunc(GL_ALWAYS, 1, 1);

                // First pass. Increase the stencil values where there are
                // shadows
                glFrontFace(GL_CCW);
                glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
                doShadowPass_(*shapeComp, workLight);

                // Second pass. Decrease the stencil values where there are
                // shadows
                glFrontFace(GL_CW);
                glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
                doShadowPass_(*shapeComp, workLight);

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
                glDisable(GL_BLEND);

                // Pop the attributes set at the beginning of the function
                //glPopAttrib();
                //glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glDepthMask(GL_TRUE);
                glEnable(GL_LIGHTING);
                glDisable(GL_STENCIL_TEST);
                glShadeModel(GL_SMOOTH);
            }
          }

          // If the entity has children
          if((*entityitor)->getChildrenList().size() > 0) {
            // Obtain the children structure and Render recursively
            update((*entityitor)->getChildrenList());        
          }

        // Pop modelview matrix
        glPopMatrix();

      }
    }
  }

}
// EO Namespace