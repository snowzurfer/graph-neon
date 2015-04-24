
// Includes
#include <shadowing_sys.h>
#include <shadow_comp.h>
#include <lnfw/physics/transform.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <tools/crc.h>
#include <base_renderer_comp.h>
#include <tools/abertay_framework.h>
#include <tools/face.h>
#include <base_renderer_comp.h>

#define PROJ_INFINITY 100.f

namespace winapp {

  void ShadowingSys::update(const std::list<lnfw::Entity *> &entities) {
    // For each light in the scene
    for(unsigned int lightNum = 0; 
      lightNum < lights_.size(); ++lightNum) {

      // Iterate through the entities and render them
      for(constEntitiesItor_ entityitor = entities.begin(); entityitor != entities.end(); ++entityitor) {
        // If the entity has a renderer component and a shadow component
        if((*entityitor)->hasComp(abfw::CRC::GetICRC("BaseRendererComp")) && 
           (*entityitor)->hasComp(abfw::CRC::GetICRC("ShadowComp"))) { 
          // Retrieve the necessary components
          ShapeComp *shapeComp = (ShapeComp *)(*entityitor)->getComp(abfw::CRC::GetICRC("ShapeComp"));
          ShadowComp *shadowComp = (ShadowComp *)(*entityitor)->getComp(abfw::CRC::GetICRC("ShadowComp"));
          const lnfw::Transform<Vec3> &transform = (*entityitor)->transform;
          
          // Retrieve the light
          const Light &light = *lights_[lightNum];

          // Push the modelview matrix
          glMatrixMode(GL_MODELVIEW);
          glPushMatrix();

            glEnable(GL_NORMALIZE);

            // Apply the geometry transformations before to apply shadowing, as the
            // same way as in the rendering system
            setupRendering(&transform, shapeComp); 

           
              
            // Work variables
            GLmatrix16f Minv;
            GLvector4f wlp, lp;

            // Compute the position with respect to the entity's local
            // coordinates system
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            // Apply transformations in inverse order
            glRotatef(-transform.rotation.getZ(), 0.f, 0.f, 1.f);
				    glRotatef(-transform.rotation.getY(), 0.f, 1.f, 0.f);
            glRotatef(-transform.rotation.getX(), 1.f, 0.f, 0.f);

            glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix From Minv
            lp[0] = light.getPosition()[0];								// Store Light Position X In lp[0]
            lp[1] = light.getPosition()[1];								// Store Light Position Y In lp[1]
            lp[2] = light.getPosition()[2];								// Store Light Position Z In lp[2]
            lp[3] = light.getPosition()[3];								// Store Light Direction In lp[3]
            vMat4Mult_(Minv, lp);									// Store Rotated Light Vector In 'lp' Array
          
            glTranslatef(-transform.position.getX(),
              -transform.position.getY(),
              -transform.position.getZ());
            glGetFloatv(GL_MODELVIEW_MATRIX, Minv);				// Retrieve ModelView Matrix From Minv
            wlp[0] = 0.0f;										// World Local Coord X To 0
            wlp[1] = 0.0f;										// World Local Coord Y To 0
            wlp[2] = 0.0f;										// World Local Coord Z To 0
            wlp[3] = 1.0f;
            vMat4Mult_(Minv, wlp);								// Store The Position Of The World Origin Relative To The
                                                  // Local Coord. System In 'wlp' Array
            lp[0] += wlp[0];									// Adding These Two Gives Us The
            lp[1] += wlp[1];									// Position Of The Light Relative To
            lp[2] += wlp[2];									// The Local Coordinate System
            glPopMatrix();

            // Create a light with position in the object's local coordinates
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
              GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT |
              GL_LIGHTING_BIT);
            // Enable face culling
            glEnable(GL_CULL_FACE);
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
            // Set the stencil function
            glStencilFunc(GL_ALWAYS, 0, 0);

            // First pass. Increase the stencil values where there are
            // shadows
            glCullFace(GL_FRONT);
            //glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
            glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
            doShadowPass_(*shapeComp, workLight);

            // Second pass. Decrease the stencil values where there are
            // shadows
            glCullFace(GL_BACK);
            //glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
            glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
            doShadowPass_(*shapeComp, workLight);

            // Enable rendering to color buffer and reset face rendering
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            // Enable depth buffer
            glDepthMask(GL_TRUE);
            // Disable face culling

            // Set the stencil to not change
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            glPopAttrib();

            glDisable(GL_NORMALIZE);

            // If the entity has children
            if((*entityitor)->getChildrenList().size() > 0) {
              // Obtain the children structure and render shadows recursively
              update((*entityitor)->getChildrenList());        
            }

          // Pop modelview matrix
          glPopMatrix();
        }        
      }


      // Clear the stencil buffer
      //glClear(GL_STENCIL_BUFFER_BIT);
    }

    

    
  }

  void ShadowingSys::doShadowPass_(const ShapeComp &shapeComp, const Light &light) {
    // Create a temporary vector containing the 

    // For each face in the shape
    for(int faceNum = 0; faceNum < shapeComp.getFaces().size(); ++faceNum) {
      // Retrieve the current face
      const Face &face = shapeComp.getFaces()[faceNum];
      

      // If the current face is visible
      if(face.visible_) {

        glPushMatrix();

        glScalef(0.99f, 0.99f, 0.99f);

        // Render the face
        glBegin(GL_TRIANGLES);
        glVertex3f(shapeComp.getVertices()[face.vertexIndices_[0]].getX(), 
          shapeComp.getVertices()[face.vertexIndices_[0]].getY(), 
          shapeComp.getVertices()[face.vertexIndices_[0]].getZ());
        glVertex3f(shapeComp.getVertices()[face.vertexIndices_[1]].getX(), 
          shapeComp.getVertices()[face.vertexIndices_[1]].getY(), 
          shapeComp.getVertices()[face.vertexIndices_[1]].getZ());
        glVertex3f(shapeComp.getVertices()[face.vertexIndices_[2]].getX(), 
          shapeComp.getVertices()[face.vertexIndices_[2]].getY(), 
          shapeComp.getVertices()[face.vertexIndices_[2]].getZ());

        glEnd();
        glPopMatrix();

        // For each edge of the face
        for(int e = 0; e < 3; ++e) {
          // Retrieve eventual neighbour
          int neighbourIndex = face.neighIndices_[e];

          // If there isn't a neighbour, or the neighbour isn't visible
          if((neighbourIndex  == 0) || 
            shapeComp.getFaces()[neighbourIndex - 1].visible_ == false) 
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

            // Add the newly created extruded vertices to the vector
            backFacesVertices_.push_back(vC);
            backFacesVertices_.push_back(vD);
          }
        }

        // Render the face
        
        glBegin(GL_TRIANGLES);
        glVertex3f((shapeComp.getVertices()[face.vertexIndices_[0]].getX() - light.getPosition()[0]) * PROJ_INFINITY, 
          (shapeComp.getVertices()[face.vertexIndices_[0]].getY()- light.getPosition()[1]) * PROJ_INFINITY, 
          (shapeComp.getVertices()[face.vertexIndices_[0]].getZ()- light.getPosition()[2]) * PROJ_INFINITY);
        glVertex3f((shapeComp.getVertices()[face.vertexIndices_[1]].getX()- light.getPosition()[0]) * PROJ_INFINITY, 
          (shapeComp.getVertices()[face.vertexIndices_[1]].getY()- light.getPosition()[1]) * PROJ_INFINITY, 
          (shapeComp.getVertices()[face.vertexIndices_[1]].getZ()- light.getPosition()[2]) * PROJ_INFINITY);
        glVertex3f((shapeComp.getVertices()[face.vertexIndices_[2]].getX()- light.getPosition()[0]) * PROJ_INFINITY, 
          (shapeComp.getVertices()[face.vertexIndices_[2]].getY()- light.getPosition()[1]) * PROJ_INFINITY, 
          (shapeComp.getVertices()[face.vertexIndices_[2]].getZ()- light.getPosition()[2]) * PROJ_INFINITY);
        
        glEnd();
      }
      //else {
      //  // Render the face
      //  glBegin(GL_TRIANGLES);
      //  glVertex3f(shapeComp.getVertices()[face.vertexIndices_[0]].getX(), 
      //    shapeComp.getVertices()[face.vertexIndices_[0]].getY(), 
      //    shapeComp.getVertices()[face.vertexIndices_[0]].getZ());
      //  glVertex3f(shapeComp.getVertices()[face.vertexIndices_[1]].getX(), 
      //    shapeComp.getVertices()[face.vertexIndices_[1]].getY(), 
      //    shapeComp.getVertices()[face.vertexIndices_[1]].getZ());
      //  glVertex3f(shapeComp.getVertices()[face.vertexIndices_[2]].getX(), 
      //    shapeComp.getVertices()[face.vertexIndices_[2]].getY(), 
      //    shapeComp.getVertices()[face.vertexIndices_[2]].getZ());

      //  glEnd();

      //}
    }

    // Render the back of the shadow volume
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < backFacesVertices_.size(); ++i) {
      glVertex3f(backFacesVertices_[i].getX(), backFacesVertices_[i].getY(), backFacesVertices_[i].getZ());
    }
    glEnd();

    // Clear the work vector
    backFacesVertices_.clear();
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

  void generateShadowMatrix_(GLmatrix16f matrix, GLvector4f light_pos, const Vec3 &normal, const Vec3 &planePt) {

    //Equation of plane is ax + by + cz = d
    //a, b and c are the coefficients of the normal to the plane (i.e. normal = ai + bj + ck)
    //If (x0, y0, z0) is any point on the plane, d = a*x0 + b*y0 + c*z0
    //i.e. d is the dot product of any point on the plane (using P here) and the normal to the plane
    float a, b, c, d;
    a = normal.getX();
    b = normal.getY();
    c = normal.getZ();
    d = normal.dot(planePt);

    //Origin of projection is at x, y, z. Projection here originating from the light source's position
    float x, y, z;

    x = light_pos[0];
    y = light_pos[1];
    z = light_pos[2];

    //This is the general perspective transformation matrix from a point (x, y, z) onto the plane ax + by + cz = d
    matrix[0] = d - (b * y + c * z);
    matrix[1] = a * y;
    matrix[2] = a * z;
    matrix[3] = a;

    matrix[4] = b * x;
    matrix[5] = d - (a * x + c * z);
    matrix[6] = b * z;
    matrix[7] = b;

    matrix[8] = c * x;
    matrix[9] = c * y;
    matrix[10] = d - (a * x + b * y);
    matrix[11] = c;

    matrix[12] = -d * x;
    matrix[13] = -d * y;
    matrix[14] = -d * z;
    matrix[15] = -(a * x + b * y + c * z);
  }
}
// EO Namespace