
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
#include <SOIL.h>
#include <cassert>
#include <winapp_colour.h>

namespace winapp {

  lnfw::Entity *EntitiesFactory::createCone(std::vector<Light *> &lights) {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    ShapeComp *shape = shapeBuilder.buildCylinder(30);
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
    transform->scale.set(10.f, 2.f, 10.f);
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

  lnfw::Entity *EntitiesFactory::createMainRoom(std::vector<Light *> &lights) {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/wizard_house/wizard_house.obj");
    ptrToShape->invertNormals();
    ptrToShape->setRenderingDir(GL_CW);

    // Load skybox texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/Models/wizard_house/wizardohouseTempTex1024.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    // If the texture has been loaded
    assert(roomTexture != 0);

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(6.f, 6.f, 6.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    testMaterial->setSpecular(0.8f, 0.8f, 0.8f, 1.f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    ShadowComp *shadowComp = new ShadowComp(lights);
    VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(shadowComp);
    entity->attachComp(velComp);

    // Return it
    return entity;
  }


  lnfw::Entity *EntitiesFactory::createTexturedCube(std::vector<Light *> &lights) {
    // Create a shape builder
    ShapesFactory shapeBuilder;

    ShapeComp *shape = shapeBuilder.buildCube(0);
    //shape->invertNormals();

    // Load skybox texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
      "media/Models/wizard_house/wizardohouseTempTex1024.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | 
      SOIL_FLAG_NTSC_SAFE_RGB | 
      SOIL_FLAG_COMPRESS_TO_DXT
      );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, 0.f);
    transform->scale.set(2.f, 2.f, 2.f);
    MaterialComp *material = new MaterialComp();
    material->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    material->setSpecular(0.8f, 0.8f, 0.8f, 1.f);
    ShadowComp *shadowComp = new ShadowComp(lights);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();

    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(material);
    entity->attachComp(shape);
    entity->transform = *transform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(testTextComp);
    entity->attachComp(shadowComp);

    // Return it
    return entity;
  }


  lnfw::Entity *EntitiesFactory::createMetallicDisk() {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    ShapeComp *shape = shapeBuilder.buildDisk(25);
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->position.set(10.f, 10.f, 3.f);
    transform->scale.set(4.f, 4.f, 4.f);
    transform->rotation.setY(270.f);
    MaterialComp *material = new MaterialComp();
    material->setAmbient(0.7f, 0.7f, 0.7f, 1.0f);
    material->setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
    material->setSpecular(1.f, 1.f, 1.f, 1.0f);
    material->setShininess(kHighShininess);
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

  lnfw::Entity *EntitiesFactory::createSandTimer() {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    float sandTimerScale = 0.3f;
    float radiusBasis = 18.f;
    float heightCones = 14.f;
    float heightBasis = 3.f;
    float distCones = heightCones - 1.f;
    float distBasis = distCones + (heightBasis / 2.f);
    float radiusCones = 16.f;
    

    // Sandtimer entity
    lnfw::Entity *sandTimer = new lnfw::Entity();
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->position.set(-10.f, 10.f, 0.f);
    transform->scale.set(sandTimerScale, sandTimerScale, sandTimerScale);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    // Add components to entity
    sandTimer->transform = *transform;
    sandTimer->attachComp(vertexRendererComp);


    // Bottom of sandtimer
    ShapeComp *shape = shapeBuilder.buildCylinder(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, -distBasis, 0.f);
    transform->scale.set(radiusBasis, heightBasis, radiusBasis);
    MaterialComp *material = new MaterialComp();
    material->setAmbient(0.7f, 0.7f, 0.7f, 1.0f);
    material->setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
    material->setSpecular(1.f, 1.f, 1.f, 1.0f);
    material->setShininess(kHighShininess);
    vertexRendererComp = new VertexRendererComp();
    // Add components to entity
    lnfw::Entity *bottom = new lnfw::Entity();
    bottom->attachComp(material);
    bottom->attachComp(shape);
    bottom->transform = *transform;
    bottom->attachComp(vertexRendererComp);
    sandTimer->addChild(bottom);


    // Bottom of glass
    shape = shapeBuilder.buildCone(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, -distCones, 0.f);
    transform->scale.set(radiusCones, heightCones, radiusCones);
    material = new MaterialComp();
    material->setAmbient(0.7f, 0.7f, 0.7f, 0.2f);
    material->setDiffuse(0.7f, 0.7f, 0.7f, 0.2f);
    material->setSpecular(1.f, 1.f, 1.f, 0.8f);
    vertexRendererComp = new VertexRendererComp();
    // Add components to entity
    lnfw::Entity *bottomGlass = new lnfw::Entity();
    bottomGlass->attachComp(material);
    bottomGlass->attachComp(shape);
    bottomGlass->transform = *transform;
    bottomGlass->attachComp(vertexRendererComp);
    sandTimer->addChild(bottomGlass);

    // Top of glass
    shape = shapeBuilder.buildCone(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, distCones, 0.f);
    transform->scale.set(radiusCones, heightCones, radiusCones);
    transform->rotation.set(180.f, 0.f, 0.f);
    material = new MaterialComp();
    material->setAmbient(0.7f, 0.7f, 0.7f, 0.2f);
    material->setDiffuse(0.7f, 0.7f, 0.7f, 0.2f);
    material->setSpecular(1.f, 1.f, 1.f, 0.8f);
    vertexRendererComp = new VertexRendererComp();
    // Add components to entity
    lnfw::Entity *topGlass = new lnfw::Entity();
    topGlass->attachComp(material);
    topGlass->attachComp(shape);
    topGlass->transform = *transform;
    topGlass->attachComp(vertexRendererComp);
    sandTimer->addChild(topGlass);

    // Top of sandtimer
    shape = shapeBuilder.buildCylinder(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, distBasis, 0.f);
    transform->scale.set(radiusBasis, heightBasis, radiusBasis);
    material = new MaterialComp();
    material->setAmbient(0.7f, 0.7f, 0.7f, 1.0f);
    material->setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
    material->setSpecular(1.f, 1.f, 1.f, 1.0f);
    material->setShininess(kHighShininess);
    vertexRendererComp = new VertexRendererComp();
    // Add components to entity
    lnfw::Entity *top = new lnfw::Entity();
    top->attachComp(material);
    top->attachComp(shape);
    top->transform = *transform;
    top->attachComp(vertexRendererComp);
    sandTimer->addChild(top);

    // Return it
    return sandTimer;
  }

  lnfw::Entity *EntitiesFactory::createRotatingArchane() {



  }


}
// EO Namespace