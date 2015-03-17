
// Includes
#include <entities_factory.h>
#include <shapes_factory.h>
#include <lnfw/entities/entity.h>
#include <lnfw/physics/transform.h>
#include <texture_comp.h>
#include <material_comp.h>
#include <vertex_renderer_comp.h>
#include <classic_renderer_comp.h>
#include <base_renderer_comp.h>
#include <animated_texture_comp.h>
#include <animated_textures_sys.h>
#include <models_loader.h>
#include <velocity_comp.h>
#include <shadow_comp.h>

namespace winapp {

  lnfw::Entity *EntitiesFactory::createCone(std::vector<Light *> &lights) {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    ShapeComp *shape = shapeBuilder.buildCone(30);
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, 0.f);
    transform->scale.set(2.f, 2.f, 2.f);
    MaterialComp *material = new MaterialComp();
    ShadowComp *shadowComp = new ShadowComp(lights);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
	VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 0.f, 0.f), Vec3());

    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(material);
    entity->attachComp(shape);
    entity->transform = *transform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(shadowComp);
	entity->attachComp(velComp);

    // Return it
    return entity;
  }

  lnfw::Entity *EntitiesFactory::createBoxRoom() {
    // Create a shape builder
    ShapesFactory shapeBuilder;

    ShapeComp *shape = shapeBuilder.buildCube(1);
    //shape->invertNormals();

    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, 0.f);
    transform->scale.set(2.f, 2.f, 5.f);
    MaterialComp *material = new MaterialComp();
    material->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    material->setSpecular(0.8f, 0.8f, 0.8f, 1.f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();

    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(material);
    entity->attachComp(shape);
    entity->transform = *transform;
    entity->attachComp(vertexRendererComp);
    
    // Return it
    return entity;
  }

}
// EO Namespace