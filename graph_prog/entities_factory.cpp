
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
#include <dlist_renderer_comp.h>
#include <models_loader.h>
#include <velocity_comp.h>
#include <shadow_comp.h>
#include <SOIL.h>
#include <cassert>
#include <winapp_colour.h>
#include <aabb_comp.h>
#include <nomatrenderer_comp.h>
#include <tools/abertay_framework.h>
#include <tools/crc.h>

namespace winapp {

  lnfw::Entity *EntitiesFactory::createCone(std::vector<Light *> &lights) {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;
      
    ShapeComp *shape = shapeBuilder.buildCylinder(30);
    shape->createDList();
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, 0.f);
    transform->scale.set(9.f, 9.f, 9.f);
    MaterialComp *material = new MaterialComp();
    ShadowComp *shadowComp = new ShadowComp();
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
	  VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(45.f, 25.f, 0.f), Vec3());

    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(material);
    entity->attachComp(shape);
    entity->transform = *transform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(shadowComp);
	  entity->attachComp(velComp);

    delete transform;

    // Return it
    return entity;
  }

  lnfw::Entity *EntitiesFactory::createBoxRoom() {
    // Create a shape builder
    ShapesFactory shapeBuilder;

    ShapeComp *shape = shapeBuilder.buildCube(1);
    //shape->invertNormals();

    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    //transform->position.set(23.f, 0.f, 0.f);
    transform->position.set(0.f, 0.f, 0.f);
    transform->scale.set(30.f, 2.f, 30.f);
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

    delete transform;
    
    // Return it
    return entity;
  }

  lnfw::Entity *EntitiesFactory::createMainRoom(std::vector<Light *> &lights) {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/wizard_house/wizardhouse_lowpoly.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);
    ptrToShape->createDList();

    // Load skybox texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/wizard_house/wizardohouseTempTex1024.png",
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
    testTransform->scale.set(35.f, 70.f, 35.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    testMaterial->setSpecular(0.8f, 0.8f, 0.8f, 1.f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    //ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    //entity->attachComp(shadowComp);
    //entity->attachComp(velComp);

    delete testTransform;

    // Return it
    return entity;
  }

  lnfw::Entity *EntitiesFactory::createPlane(const Vec3 &colour) {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    // Main entity
    lnfw::Entity *ent = new lnfw::Entity(abfw::CRC::GetICRC("Plane"));
    ShapeComp *shape = shapeBuilder.buildPlane(0);
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->scale.set(1.f, 1.f, 1.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setDiffuse(colour.getX(), colour.getY(), colour.getZ(), 1.f);
    testMaterial->setAmbient(colour.getX(), colour.getY(), colour.getZ(), 1.f);
    //testMaterial->setSpecular(kBlackColourNoAlpha);
    testMaterial->setShininess(kHighShininess);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    //ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(shape->getVertices());
    //VelocityComp *velComp = new VelocityComp();
    //lnfw::Transform<Vec3> *velTransform = new lnfw::Transform<Vec3>();
    //velComp->setTransform(*velTransform);
    //delete velTransform;
    // Add components to entity

    ent->transform = *transform;
    ent->attachComp(vertexRendererComp);
    ent->attachComp(testMaterial);
    ent->attachComp(shape);
    //ent->attachComp(shadowComp);
    //ent->attachComp(aabbComp);
    //ent->attachComp(velComp);

    delete transform;

    return ent;
  }


  lnfw::Entity *EntitiesFactory::createSandCone() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/doublecone.0002.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load skybox texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/sand_texture1015.jpg",
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
    testTransform->scale.set(1.f, 1.f, 1.f);
    testTransform->rotation.setY(90.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    testMaterial->setSpecular(0.8f, 0.8f, 0.8f, 1.f);
    BaseRendererComp *vertexRendererComp = new NoMatRendererComp();
    AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.3f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);
    //ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(animTextureComp);
    //entity->attachComp(shadowComp);
    //entity->attachComp(velComp);
    delete testTransform;
    

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
      "media/models/wizard_house/wizardohouseTempTex1024.png",
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
    ShadowComp *shadowComp = new ShadowComp();
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();

    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(material);
    entity->attachComp(shape);
    entity->transform = *transform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(testTextComp);
    entity->attachComp(shadowComp);

    delete transform;

    // Return it
    return entity;
  }


  lnfw::Entity *EntitiesFactory::createMetallicDisk(const Vec3 &col, const float scale) {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    ShapeComp *shape = shapeBuilder.buildDisk(25);
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->scale.set(scale, scale, scale);
    //transform->rotation.setY(90.f);
    MaterialComp *material = new MaterialComp();
    material->setAmbient(col.getX(), col.getY(), col.getZ(), 1.0f);
    material->setDiffuse(col.getX(), col.getY(), col.getZ(), 1.0f);
    material->setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
    //material->setShininess(k);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    //ShadowComp *shadowComp = new ShadowComp();
    lnfw::AABBComp *aabbComp = new lnfw::AABBComp(shape->getVertices());

    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity(abfw::CRC::GetICRC("Disk"));
    entity->attachComp(material);
    entity->attachComp(shape);
    entity->transform = *transform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(aabbComp);

    delete transform;

    // Return it
    return entity;
  }

  lnfw::Entity *EntitiesFactory::createSandTimer() {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    float sandTimerScale = 1.5f;
    float radiusBasis = 18.f;
    float heightCones = 14.f;
    float heightBasis = 3.f;
    float distCones = heightCones - 1.f;
    float distBasis = distCones + (heightBasis / 2.f);
    float radiusCones = 16.f;
    float distGlass = 10.f;
    

    // Sandtimer entity
    lnfw::Entity *sandTimer = new lnfw::Entity();
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    //transform->position.set(-10.f, 10.f, 0.f);
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
    vertexRendererComp = new NoMatRendererComp();
    // Add components to entity
    lnfw::Entity *bottom = new lnfw::Entity();
    bottom->attachComp(material);
    bottom->attachComp(shape);
    bottom->transform = *transform;
    bottom->attachComp(vertexRendererComp);
    sandTimer->addChild(bottom);


    // Top of sandtimer
    shape = shapeBuilder.buildCylinder(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, distBasis, 0.f);
    transform->scale.set(radiusBasis, heightBasis, radiusBasis);
    material = new MaterialComp();
    material->setAmbient(0.73f, 0.73f, 0.73f, 1.0f);
    material->setDiffuse(0.73f, 0.73f, 0.73f, 1.0f);
    material->setSpecular(1.f, 1.f, 1.f, 1.0f);
    material->setShininess(kHighShininess);
    vertexRendererComp = new NoMatRendererComp();
    // Add components to entity
    lnfw::Entity *top = new lnfw::Entity();
    top->attachComp(material);
    top->attachComp(shape);
    top->transform = *transform;
    top->attachComp(vertexRendererComp);
    sandTimer->addChild(top);


    // Moving sand inside
    lnfw::Entity *movSand = createSandCone();
    movSand->transform.scale.set(1.6f, 1.8f, 1.6f);
    movSand->transform.position.setY(-6.f);
    sandTimer->addChild(movSand);

    // Bottom of glass
    shape = shapeBuilder.buildCone(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, -distCones, 0.f);
    transform->scale.set(radiusCones, heightCones, radiusCones);
    material = new MaterialComp();
    material->setAmbient(0.9f, 0.9f, 0.9f, 0.2f);
    material->setDiffuse(0.9f, 0.9f, 0.9f, 0.2f);
    material->setSpecular(1.f, 1.f, 1.f, 0.8f);
    vertexRendererComp = new NoMatRendererComp();
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
    material->setAmbient(0.9f, 0.9f, 0.9f, 0.2f);
    material->setDiffuse(0.9f, 0.9f, 0.9f, 0.2f);
    material->setSpecular(1.f, 1.f, 1.f, 0.8f);
    vertexRendererComp = new NoMatRendererComp();
    // Add components to entity
    lnfw::Entity *topGlass = new lnfw::Entity();
    topGlass->attachComp(material);
    topGlass->attachComp(shape);
    topGlass->transform = *transform;
    topGlass->attachComp(vertexRendererComp);
    sandTimer->addChild(topGlass);

    


    // Glass in front of sandtimer
    shape = shapeBuilder.buildPlane(0);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, distGlass);
    transform->scale.set(radiusBasis , heightCones * 2 + heightBasis * 2, 1.f);
    material = new MaterialComp();
    material->setAmbient(0.6f, 0.6f, 0.6f, 0.2f);
    material->setDiffuse(0.6f, 0.6f, 0.6f, 0.2f);
    material->setSpecular(1.f, 1.f, 1.f, 0.8f);
    vertexRendererComp = new NoMatRendererComp();
    // Add components to entity
    lnfw::Entity *glass = new lnfw::Entity();
    glass->attachComp(material);
    glass->attachComp(shape);
    glass->transform = *transform;
    glass->attachComp(vertexRendererComp);
    sandTimer->addChild(glass);


    delete transform;

    // Return it
    return sandTimer;
  }

  lnfw::Entity *EntitiesFactory::createRotatingArchane() {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    //float archaneScale = 0.3f;
    float mainRadius = 18.f;
    float heightBasis = 2.f;
    float armDist = mainRadius / 2.f;
    float armWidth = 2.f;
    float armLenght = mainRadius - 0.5f;

    // Main entity
    lnfw::Entity *archane = new lnfw::Entity();
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->scale.set(1.f, 1.f, 1.f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    VelocityComp *velComp = new VelocityComp();
    lnfw::Transform<Vec3> *velTransform = new lnfw::Transform<Vec3>();
    velComp->setTransform(*velTransform);
    delete velTransform;
    // Add components to entity
    archane->transform = *transform;
    archane->attachComp(vertexRendererComp);
    archane->attachComp(velComp);


    // Pivot
    lnfw::Entity *pivot = new lnfw::Entity();
    transform = new lnfw::Transform<Vec3>();
    transform->scale.set(1, 1, 1);
    vertexRendererComp = new VertexRendererComp();
    velComp = new VelocityComp();
    velTransform = new lnfw::Transform<Vec3>();
    velTransform->rotation.setY(25.f);
    velComp->setTransform(*velTransform);
    delete velTransform;
    // Add components to entity
    pivot->transform = *transform;
    pivot->attachComp(vertexRendererComp);
    pivot->attachComp(velComp);
    archane->addChild(pivot);


    // Rotating arm
    ShapeComp *shape = shapeBuilder.buildCube(0);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, armDist);
    transform->scale.set(armWidth, heightBasis, armLenght);
    MaterialComp *material = new MaterialComp();
    material->setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    material->setDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
    material->setShininess(kHighShininess);
    vertexRendererComp = new NoMatRendererComp();
    // Add components to entity
    lnfw::Entity *arm = new lnfw::Entity();
    arm->attachComp(material);
    arm->attachComp(shape);
    arm->transform = *transform;
    arm->attachComp(vertexRendererComp);
    pivot->addChild(arm);


    // Arm's disk
    shape = shapeBuilder.buildCylinder(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, mainRadius - 1.f);
    transform->scale.set(3.5f, 1.f, 3.5f);
    material = new MaterialComp();
    material->setAmbient(0.5f, 0.647059f, 0.5f, 1.0f);
    material->setDiffuse(0.5f, 0.647059f, 0.5f, 1.0f);
    material->setSpecular(1.f, 1.f, 1.f, 1.0f);
    //material->setShininess(kHighShininess);
    vertexRendererComp = new NoMatRendererComp();
    lnfw::AABBComp *aabbComp = new lnfw::AABBComp(shape->getVertices());
    // Add components to entity
    lnfw::Entity *armDisk = new lnfw::Entity(abfw::CRC::GetICRC("Cylinder"));
    armDisk->attachComp(material);
    armDisk->attachComp(shape);
    armDisk->transform = *transform;
    armDisk->attachComp(vertexRendererComp);
    armDisk->attachComp(aabbComp);
    pivot->addChild(armDisk);


    // Rotating cylinder
    shape = shapeBuilder.buildCylinder(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.set(0.f, 0.f, -(mainRadius - 1.f));
    transform->scale.set(3.5f, 0.3f, 3.5f);
    material = new MaterialComp();
    material->setAmbient(0.580392f, 0.5f, 0.827451f, 1.0f);
    material->setDiffuse(0.580392f, 0.5f, 0.827451f, 1.0f);
    material->setSpecular(1.f, 1.f, 1.f, 1.0f);
    //material->setShininess(kHighShininess);
    vertexRendererComp = new NoMatRendererComp();
    velComp = new VelocityComp();
    velTransform = new lnfw::Transform<Vec3>();
    velTransform->rotation.setZ(180.f);
    velComp->setTransform(*velTransform);
    //aabbComp = new lnfw::AABBComp(shape->getVertices());
    delete velTransform;
    // Add components to entity
    lnfw::Entity *disk = new lnfw::Entity(abfw::CRC::GetICRC("Cylinder"));
    disk->attachComp(material);
    disk->attachComp(shape);
    disk->transform = *transform;
    disk->attachComp(vertexRendererComp);
    disk->attachComp(velComp);
   // disk->attachComp(aabbComp);
    pivot->addChild(disk);

    // Rotating disk
    shape = shapeBuilder.buildDisk(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.setY((heightBasis / 2.f) + 0.1f); 
    transform->scale.set(6.f, 6.f, 6.f);
    transform->rotation.setX(90.f);
    material = new MaterialComp();
    material->setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    material->setDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
    material->setSpecular(1.f, 1.f, 1.f, 1.0f);
    //material->setShininess(kHighShininess);
    vertexRendererComp = new NoMatRendererComp();
    velComp = new VelocityComp();
    velTransform = new lnfw::Transform<Vec3>();
    velTransform->rotation.setY(-35.f);
    velComp->setTransform(*velTransform);
    delete velTransform;
    // Add components to entity
    lnfw::Entity *coin = new lnfw::Entity();
    coin->attachComp(material);
    coin->attachComp(shape);
    coin->transform = *transform;
    coin->attachComp(vertexRendererComp);
    coin->attachComp(velComp);
    pivot->addChild(coin);

    // Glass pavement on top
    shape = shapeBuilder.buildDisk(25);
    transform = new lnfw::Transform<Vec3>();
    transform->position.setY((heightBasis / 2.f) + 1.f); 
    transform->rotation.setX(270.f);
    transform->scale.set(mainRadius * 2, mainRadius * 2, 1.f);
    material = new MaterialComp();
    material->setAmbient(0.9f, 0.9f, 0.9f, 0.2f);
    material->setDiffuse(0.9f, 0.9f, 0.9f, 0.2f);
    material->setSpecular(1.f, 1.f, 1.f, 0.8f);
    //material->setShininess(kHighShininess);
    vertexRendererComp = new NoMatRendererComp();
    // Add components to entity
    lnfw::Entity *glass = new lnfw::Entity();
    glass->attachComp(material);
    glass->attachComp(shape);
    glass->transform = *transform;
    glass->attachComp(vertexRendererComp);
    archane->addChild(glass);


    delete transform;

    return archane;

  }

  lnfw::Entity *EntitiesFactory::createTeapot() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/teapot.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    GLuint text = 0;
    text = SOIL_load_OGL_texture  (
      "media/Models/crate.png",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | 
      SOIL_FLAG_NTSC_SAFE_RGB | 
      SOIL_FLAG_COMPRESS_TO_DXT
      );

    // If the texture has been loaded
    assert(text != 0);

    TextureComp *testTextComp = new TextureComp(text);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(0.8f, 0.2f, 0.f, 1.f);
    testMaterial->setDiffuse(0.8f, 0.2f, 0.f, 1.f);
    testMaterial->setSpecular(1.f, 1.f, 1.f, 1.f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(3.f, 0.0f), Texel(0.f, 20.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);
    //ShadowComp *shadowComp = new ShadowComp();
    VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 25.f, 25.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity(abfw::CRC::GetICRC("Teapot"));
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    //entity->attachComp(shadowComp);
    //entity->attachComp(aabbComp);
    entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }

  lnfw::Entity *EntitiesFactory::createStonePillar() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/stone_fillars/stone_fillars.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/stone_fillars/handpaintedstonepillars.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    //testTransform->position.set(5.f, 5.f, 5.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(kBlackColourNoAlpha);
    testMaterial->setDiffuse(0.64f, 0.64f, 0.64f, 1.f);
    testMaterial->setSpecular(0.5f, 0.5f, 0.5f, 1.f);
    testMaterial->setShininess(96.078431f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    //ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    //entity->attachComp(shadowComp);
    //entity->attachComp(aabbComp);
    //entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }

  lnfw::Entity *EntitiesFactory::createChair() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/chair/chair.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/chair/mohogany_light.jpg",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    //testTransform->position.set(5.f, 5.f, 5.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(kBlackColourNoAlpha);
    testMaterial->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    testMaterial->setSpecular(0.5f, 0.5f, 0.5f, 1.f);
    testMaterial->setShininess(96.078431f);
    BaseRendererComp *vertexRendererComp = new ClassicRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    ShadowComp *shadowComp = new ShadowComp();
    VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(25.f, 25.f, 0.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(shadowComp);
    //entity->attachComp(aabbComp);
    entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }

  lnfw::Entity *EntitiesFactory::createOttoman() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/ottoman/cassapanca.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/ottoman/cassapanca.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    //testTransform->position.set(5.f, 5.f, 5.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(kBlackColourNoAlpha);
    testMaterial->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    testMaterial->setSpecular(kBlackColourNoAlpha);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(shadowComp);
    //entity->attachComp(aabbComp);
    //entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }

  lnfw::Entity *EntitiesFactory::createDesk() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/desk/table_medieval_trestle_0.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/desk/med_table.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    //testTransform->position.set(5.f, 5.f, 5.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(kBlackColourNoAlpha);
    testMaterial->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    testMaterial->setSpecular(kBlackColourNoAlpha);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    //ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    //entity->attachComp(shadowComp);
    //entity->attachComp(aabbComp);
    //entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }


  lnfw::Entity *EntitiesFactory::createBookShelf() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/bookshelf/bookshelf.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/bookshelf/mohogany_light.jpg",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    //testTransform->position.set(5.f, 5.f, 5.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(kBlackColourNoAlpha);
    testMaterial->setDiffuse(0.8f, 0.8f, 0.8f, 1.f);
    testMaterial->setSpecular(0.5f, 0.5f, 0.5f, 1.f);
    testMaterial->setShininess(96.078431f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    //ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    //entity->attachComp(shadowComp);
    //entity->attachComp(aabbComp);
    //entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }


  lnfw::Entity *EntitiesFactory::createSkull() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/human_skull_low_res/human_skull_low_res.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/human_skull_low_res/human_skull_aged_dif.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    //testTransform->position.set(5.f, 5.f, 5.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(kBlackColourNoAlpha);
    testMaterial->setDiffuse(0.64f, 0.64f, 0.64f, 1.f);
    testMaterial->setSpecular(1.f, 1.f, 1.f, 1.f);
    testMaterial->setShininess(194.117647f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    ShadowComp *shadowComp = new ShadowComp();
    VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(40.f, 25.f, 0.f), Vec3());
    //lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity();
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(shadowComp);
    //entity->attachComp(aabbComp);
    entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }


  lnfw::Entity *EntitiesFactory::createPot() {
    // Create a models loader
    ModelsLoader modelsLoader;

	  ShapeComp *ptrToShape = modelsLoader.load("media/models/cooking_pot/cooking_pot.obj");
    //ptrToShape->invertNormals();
    //ptrToShape->setRenderingDir(GL_CW);

    // Load texture
    GLuint roomTexture = 0;
    roomTexture = SOIL_load_OGL_texture  (
                        "media/models/cooking_pot/cooking_pot.png",
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS | 
                        SOIL_FLAG_NTSC_SAFE_RGB | 
                        SOIL_FLAG_COMPRESS_TO_DXT
    );

    TextureComp *testTextComp = new TextureComp(roomTexture);
    lnfw::Transform<Vec3> *testTransform = new lnfw::Transform<Vec3>();
    testTransform->scale.set(1.f, 1.f, 1.f);
    //testTransform->position.set(5.f, 5.f, 5.f);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setAmbient(kBlackColourNoAlpha);
    testMaterial->setDiffuse(0.64f, 0.64f, 0.64f, 1.f);
    testMaterial->setSpecular(0.5f, 0.5f, 0.5f, 1.f);
    testMaterial->setShininess(96.078431f);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    /*AnimatedTextureComp *animTextureComp = new AnimatedTextureComp();
    lnfw::Transform<Texel> animTextTransform(Texel(0.f, 0.0f), Texel(0.f, 0.f), Texel(0.f, 0.f));
    animTextureComp->setTransform(animTextTransform);*/
    ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
    lnfw::AABBComp *aabbComp = new lnfw::AABBComp(ptrToShape->getVertices());
  
    // Add components to entity
    lnfw::Entity *entity = new lnfw::Entity(abfw::CRC::GetICRC("Pot"));
    entity->attachComp(testMaterial);
    entity->attachComp(ptrToShape);
    entity->attachComp(testTextComp);
    entity->transform = *testTransform;
    entity->attachComp(vertexRendererComp);
    entity->attachComp(shadowComp);
    entity->attachComp(aabbComp);
    //entity->attachComp(velComp);


    delete testTransform;

    // Return it
    return entity;

  }


  lnfw::Entity *EntitiesFactory::createMaterialSphere(const Vec3 &colour, const float radius) {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    // Main entity
    lnfw::Entity *ent = new lnfw::Entity(abfw::CRC::GetICRC("MatSphere"));
    ShapeComp *shape = shapeBuilder.buildCubeSphere(3);
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->scale.set(radius, radius, radius);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setDiffuse(colour.getX(), colour.getY(), colour.getZ(), 1.f);
    testMaterial->setAmbient(colour.getX(), colour.getY(), colour.getZ(), 1.f);
    //testMaterial->setSpecular(kBlackColourNoAlpha);
    //testMaterial->setShininess(kHighShininess);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    ShadowComp *shadowComp = new ShadowComp();
    //VelocityComp *velComp = new VelocityComp(Vec3(), Vec3(0.f, 1.f, 0.f), Vec3());
    lnfw::AABBComp *aabbComp = new lnfw::AABBComp(shape->getVertices());
    //VelocityComp *velComp = new VelocityComp();
    //lnfw::Transform<Vec3> *velTransform = new lnfw::Transform<Vec3>();
    //velComp->setTransform(*velTransform);
    //delete velTransform;
    // Add components to entity

    ent->transform = *transform;
    ent->attachComp(vertexRendererComp);
    ent->attachComp(testMaterial);
    ent->attachComp(shape);
    ent->attachComp(shadowComp);
    ent->attachComp(aabbComp);
    //ent->attachComp(velComp);


    return ent;
  }



  lnfw::Entity *EntitiesFactory::createMaterialSpherePot() {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapeBuilder;

    const float radius = 1.f;
    const Vec3 colour(0.f, 1.f, 0.f);

    // Main entity
    lnfw::Entity *ent = new lnfw::Entity(abfw::CRC::GetICRC("MatSpherePot"));
    ShapeComp *shape = shapeBuilder.buildCubeSphere(3);
    lnfw::Transform<Vec3> *transform = new lnfw::Transform<Vec3>();
    transform->scale.set(radius, radius, radius);
    MaterialComp *testMaterial = new MaterialComp();
    testMaterial->setDiffuse(colour.getX(), colour.getY(), colour.getZ(), 1.f);
    testMaterial->setAmbient(colour.getX(), colour.getY(), colour.getZ(), 1.f);
    //testMaterial->setSpecular(kBlackColourNoAlpha);
    //testMaterial->setShininess(kHighShininess);
    BaseRendererComp *vertexRendererComp = new VertexRendererComp();
    ShadowComp *shadowComp = new ShadowComp();
    VelocityComp *velComp = new VelocityComp(Vec3(0.f, 5.f, 0.f), Vec3(), Vec3());
    lnfw::AABBComp *aabbComp = new lnfw::AABBComp(shape->getVertices());
    //VelocityComp *velComp = new VelocityComp();
    //lnfw::Transform<Vec3> *velTransform = new lnfw::Transform<Vec3>();
    //velComp->setTransform(*velTransform);
    //delete velTransform;
    // Add components to entity

    ent->transform = *transform;
    ent->attachComp(vertexRendererComp);
    ent->attachComp(testMaterial);
    ent->attachComp(shape);
    ent->attachComp(shadowComp);
    ent->attachComp(aabbComp);
    ent->attachComp(velComp);


    return ent;
  }

}
// EO Namespace