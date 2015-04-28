// NOTES:
//
//  The following raypicking system has been developed using the following code as reference:
//  https://code.google.com/p/opengl-tutorial-org/source/browse/misc05_picking/misc05_picking_custom.cpp
//  It is part of a tutorial located at this link:
//  http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
//
// END OF NOTES

// Includes
#include <raypicking_sys.h>
#include <lnfw\physics\Vector3\Vec3.h>
#include <lnfw\entities\entity.h>
#include <aabb_comp.h>
#include <cmath>
#include <iostream>
#include <material_comp.h>
#include <prev_material_comp.h>
#include <entities_factory.h>


namespace winapp {

  RaypickingSys::RaypickingSys(const Camera *camera, Input *input, std::list<lnfw::Entity *> &entitiesToAdd) :
    System(), cam_(camera), input_(input),
    entitiesToAdd_(entitiesToAdd)
  {
    srand(102156);
  }

  void RaypickingSys::update(const std::list<lnfw::Entity *> &entities) {
    // If the mouse has been clicked
    if(input_->isLPressed()) {
      // Retrieve ray's position vector
      const Vec3 &rOrigin = cam_->getPos();
      // Retrieve ray's direction vector
      const Vec3 &rDir    = cam_->getForward();
    
      // Iterate through the entities and update them
      for(constEntitiesItor_ entityitor = entities.begin(); entityitor != entities.end(); ++entityitor) {
        // If the entity has the required component(s)
        if((*entityitor)->hasComp(abfw::CRC::GetICRC("AABBComp"))) {
          // Retrieve the component(s)
          lnfw::AABBComp *aabbComp = (lnfw::AABBComp *)(*entityitor)->
            getComp(abfw::CRC::GetICRC("AABBComp"));
        
          // Read the distance in case of collision
          float intersectionDist;

          // If the ray is intersecting with the AABB
          if(testRayOOBB_(rOrigin, rDir, aabbComp, &(*entityitor)->transform, intersectionDist)) {
            // Do something
            std::cout << "Picked object!" << std::endl;

            // Depending on the object's type
            if((*entityitor)->getID() == abfw::CRC::GetICRC("Disk")) {
              // Change its colour
              MaterialComp *matComp = (MaterialComp *)(*entityitor)->
                getComp(abfw::CRC::GetICRC("MaterialComp"));

              // If it had not already been clicked
              if(!aabbComp->isClicked()) {
                aabbComp->setClicked(true);

				        // Save the current material
				        PrevMaterialComp *pMat = new PrevMaterialComp(*matComp);
                (*entityitor)->attachComp(pMat);

                matComp->setEmission(0.f, 0.f, 1.f, 1.f);
				        matComp->setAmbient(0.f, 0.f, 0.f, 1.f); 
                matComp->setDiffuse(0.1f, 0.1f, 0.1f, 1.f);
				        matComp->setSpecular(0.f, 0.f, 0.f, 1.f);
              }
              else if(aabbComp->isClicked()) {
                aabbComp->setClicked(false);

                // Restore the previous material
				        PrevMaterialComp *pMatComp = (PrevMaterialComp *)(*entityitor)->
                getComp(abfw::CRC::GetICRC("PrevMaterialComp"));

				        *matComp = pMatComp->mat;

                // Remove the component
                (*entityitor)->detachComp(abfw::CRC::GetICRC("PrevMaterialComp"));
              }
            }
            if((*entityitor)->getID() == abfw::CRC::GetICRC("Cylinder")) {
              // Change its colour
              MaterialComp *matComp = (MaterialComp *)(*entityitor)->
                getComp(abfw::CRC::GetICRC("MaterialComp"));

              // If it had not already been clicked
              if(!aabbComp->isClicked()) {
                aabbComp->setClicked(true);

                //matComp->setEmission(0.f, 0.f, 0.95f, 1.f);
                matComp->setDiffuse(0.580392f, 0.5f, 0.827451f, 1.0f);
              }
              else if(aabbComp->isClicked()) {
                aabbComp->setClicked(false);

                //matComp->setEmission(0.f, 0.f, 0.0f, 1.f);
                matComp->setDiffuse(0.5f, 0.647059f, 0.5f, 1.0f);
              }
            }
            // Depending on the object's type
            if((*entityitor)->getID() == abfw::CRC::GetICRC("Pot")) {
              // Spawn a sphere
              EntitiesFactory factory;

              lnfw::Entity *sphere = factory.createMaterialSpherePot();

              
              int random = rand() % 3;

              sphere->transform.position.set(-4.433f + (float)random, 14.f, 19.f + (float)random);

              entitiesToAdd_.push_back(sphere);

            }
            // Change colour of object
          }
          /*else {
            std::cout << std::endl;
          }*/
        }

        // If the entity has children
        //if((*entityitor)->getChildrenList().size() > 0) {
        //  // Obtain the children structure and Render recursively
        //  update((*entityitor)->getChildrenList());        
        //}
      }

      

    }
    
  }

  const bool RaypickingSys::testRayOOBB_(const Vec3 &rOrigin, const Vec3 &rDir, 
    const lnfw::AABBComp *aabbComp, const lnfw::Transform<Vec3> *transform,
    float &intersectionDist) 
  {
    // Parameters of ray at intersection. They represent the
    // smallest and largest value at intersection for ALL axis.
    float tMin = 0.f;
    float tMax = 100000.f;

    // Difference between the entity's position and the ray's origin
    const Vec3 delta = transform->position - rOrigin;

    // Calculate the scaling of the structure
    const Vec3 aabbMin = aabbComp->getMinL() * transform->scale;
    const Vec3 aabbMax = aabbComp->getMaxR() * transform->scale;

    // Axis to be checked: X
    {
      // Retrieve the plane's normal
      const Vec3 n(1.f, 0.f, 0.f);

      // Calculate the two necessary components for obtaining the paramenter T at
      // intersection
      const float e = n.dot(delta);
      const float f = rDir.dot(n);

      // If the ray is not parallel to the plane
      if(fabs(f) > 0.001f) {
        // Store the inverse of f
        const float invF = 1.f / f;

        // Calculate the intersection parameters with left and right planes.
        // The actual position of the panel within the local coordinates of the entity
        // are added to the previously calculated difference to correctly offset the 
        // plane.
        float t1 = (e + aabbMin.getX()) * invF;
        float t2 = (e + aabbMax.getX()) * invF;

        // t1 should represent the nearest intersection, so if this is not the case
        if(t1 > t2) {
          // Swap t1 with t2
          const float sw = t1;
          t1 = t2;
          t2 = sw;
        }

        // tMax represents the smallest of the intersections with the back/right/far
        // planes, considered all the x, y, z planes the algorithm checks against.
        // If t2 (this axis's back/right/far intersection point) is less than
        // the one stored alread in tMax
        if(t2 < tMax) {
          // Update tMax
          tMax = t2;
        }

        // tMin represents the largest of the intersections with the front/left/near
        // planes, considered all the x, y, z planes the algorithm checks against.
        // If t1 (this axis's front/left/near intersection point) is larger than
        // the one stored alread in tMin
        if(t1 > tMin) {
          // Update tMin
          tMin = t1;
        }

        // Due to the algorithm, if tMax is less than tMin
        if(tMax < tMin) {
          // There isn't an intersection
          return false;
        }
      }
      // If the line is almost parallel to the planes
      else {
        if((aabbMin.getX() - e) > 0.f || (aabbMax.getX() - e) < 0.f) {
          return false;
        }
      }
    }

    // Axis to be checked: Y
    // The algorithm is as the same as above
    {
      const Vec3 n(0.f, 1.f, 0.f);

      const float e = n.dot(delta);
      const float f = rDir.dot(n);

      if(fabs(f) > 0.001f) {
        const float invF = 1.f / f;

        float t1 = (e + aabbMin.getY()) * invF;
        float t2 = (e + aabbMax.getY()) * invF;

        if(t1 > t2) {
          const float sw = t1;
          t1 = t2;
          t2 = sw;
        }

        if(t2 < tMax) {
          tMax = t2;
        }
        if(t1 > tMin) {
          tMin = t1;
        }

        if(tMax < tMin) {
          return false;
        }
      }
      else {
        if((aabbMin.getY() - e) > 0.f || (aabbMax.getY() - e) < 0.f) {
          return false;
        }
      }
    }

    // Axis to be checked: Z
    // The algorithm is as the same as above
    {
      const Vec3 n(0.f, 0.f, 1.f);

      const float e = n.dot(delta);
      const float f = rDir.dot(n);

      if(fabs(f) > 0.001f) {
        const float invF = 1.f / f;

        float t1 = (e + aabbMin.getZ()) * invF;
        float t2 = (e + aabbMax.getZ()) * invF;

        if(t1 > t2) {
          const float sw = t1;
          t1 = t2;
          t2 = sw;
        }

        if(t2 < tMax) {
          tMax = t2;
        }
        if(t1 > tMin) {
          tMin = t1;
        }

        if(tMax < tMin) {
          return false;
        }
      }
      else {
        if((aabbMin.getZ() - e) > 0.f || (aabbMax.getZ() - e) < 0.f) {
          return false;
        }
      }
    }

    // Return the value of the ray parameter at intersection
    intersectionDist = tMin;

    // The ray intersects
    return true;
  }


}
// EO Namespace