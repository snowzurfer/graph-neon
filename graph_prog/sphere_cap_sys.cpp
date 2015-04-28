
// Includes
#include <sphere_cap_sys.h>
#include <lnfw/physics/transform.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <tools/crc.h>
#include <tools/abertay_framework.h>
#include <velocity_comp.h>
#include <app_globals.h>

namespace winapp {

  SphereCapSys::SphereCapSys(std::list<lnfw::Entity *> &entitiesToDelete) :
    entitiesToDelete_(entitiesToDelete) {}

  void SphereCapSys::update(const std::list<lnfw::Entity *> &entities) {
    // Iterate through the entities and update them
    for(constEntitiesItor_ entityitor = entities.begin(); entityitor != entities.end(); ++entityitor) {
      // If the entity has the required component(s)
      if((*entityitor)->hasComp(abfw::CRC::GetICRC("VelocityComp"))) { 
        // Retrieve the component(s)
        VelocityComp *velComp = (VelocityComp *)(*entityitor)->
          getComp(abfw::CRC::GetICRC("VelocityComp"));

        // Depending on the object's type
        if((*entityitor)->getID() == abfw::CRC::GetICRC("MatSpherePot")) {
          // Check its height above the pot
          if((*entityitor)->transform.position.getY() >= 30.f) {
            // Stop it
            entitiesToDelete_.push_back((*entityitor));
          }

        }

        // Integrate and update the position
        lnfw::Transform<Vec3> velocityStep = 
          velComp->getTransform() * kSecPerUpdate;
        (*entityitor)->transform = ((*entityitor)->transform + velocityStep);

        // If the entity has children
        if((*entityitor)->getChildrenList().size() > 0) {
          // Obtain the children structure and Render recursively
          update((*entityitor)->getChildrenList());        
        }

      }
    }
  }

}
// EO Namespace