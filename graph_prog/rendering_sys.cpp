
// Includes
#include <rendering_sys.h>
#include <lnfw/physics/transform.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <tools/crc.h>
#include <base_renderer_comp.h>
#include <tools/abertay_framework.h>


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