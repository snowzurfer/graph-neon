
// Includes
#include <animated_textures_sys.h>
#include <animated_texture_comp.h>
#include <texture_comp.h>
#include <tools/abertay_framework.h>
#include <tools/crc.h>
#include <lnfw/physics/transform.h>
#include <app_globals.h>

namespace winapp {

  void AnimatedTextureSys::update(const std::list<lnfw::Entity *> &entities) {
    // Iterate through the entities
    for(constEntitiesItor_ entityitor = entities.begin(); entityitor != entities.end(); ++entityitor) {
      // If the entity has the required component(s)
      if(((*entityitor)->hasComp(abfw::CRC::GetICRC("AnimatedTextureComp"))) &&
         ((*entityitor)->hasComp(abfw::CRC::GetICRC("TextureComp")))) { 
        // Retrieve the component(s)
        AnimatedTextureComp *animatedTextureComp = (AnimatedTextureComp *)(*entityitor)->
          getComp(abfw::CRC::GetICRC("AnimatedTextureComp"));
        TextureComp *textureComp = (TextureComp *)(*entityitor)->
          getComp(abfw::CRC::GetICRC("TextureComp")); 

        // Update the texture component
        lnfw::Transform<Texel> velocityStep = 
          animatedTextureComp->getTransform() * kSecPerUpdate;

        lnfw::Transform<Texel> textureTransform = textureComp->getTransform();

        textureTransform = textureTransform + velocityStep;

        // Check for overflows
        if(textureTransform.rotation.x > 360.f) {
          textureTransform.rotation.x = 0.f;
        }
        if(textureTransform.rotation.y > 360.f) {
          textureTransform.rotation.x = 0.f;
        }

        textureComp->setTransform(textureTransform );

        
        
      }

      // If the entity has children
      if((*entityitor)->getChildrenList().size() > 0) {
        // Obtain the children structure and Render recursively
        update((*entityitor)->getChildrenList());        
      }
    }
  }

}
// EO Namespace