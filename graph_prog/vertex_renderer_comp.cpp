
// Includes
#include <vertex_renderer_comp.h>
#include <Windows.h>
#include <gl/GL.h>

namespace winapp {

  //REGISTER_COMPONENT_IMPL(VertexRendererComp);
  
  const void VertexRendererComp::render(const lnfw::Transform<Vec3> *transform, 
                                        const ShapeComp *shape, 
                                        const TextureComp *texture /* = NULL */,
                                        const MaterialComp *material /* = NULL */)
  {
    // Move to position
    glTranslatef(transform->position.getX(),
                 transform->position.getY(),
                 transform->position.getZ());
    // Scale
    glScalef(transform->scale.getX(),
             transform->scale.getY(),
             transform->scale.getZ());

    // Rotate
    glRotatef(transform->rotation.getX(), 1.f, 0.f, 0.f);
    glRotatef(transform->rotation.getY(), 0.f, 1.f, 0.f);
    glRotatef(transform->rotation.getZ(), 0.f, 0.f, 1.f);

    // Set material properties for the geometry
    material->apply(GL_FRONT);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

    
    //////////RENDER

    

    // Specify data for the arrays

    float *vertices = new float[shape->getVertices().size() * 3];

    int counter = 0;
    for(int i = 0; i < shape->getVertices().size(); ++i) {
      vertices[counter++] = shape->getVertices()[i].getX();
      vertices[counter++] = shape->getVertices()[i].getY();
      vertices[counter++] = shape->getVertices()[i].getZ();
    }

    counter = 0;
    float *normals = new float[shape->getNormals().size() * 3];
    for(int i = 0; i < shape->getNormals().size(); ++i) {
      normals[counter++] = shape->getNormals()[i].getX();
      normals[counter++] = shape->getNormals()[i].getY();
      normals[counter++] = shape->getNormals()[i].getZ();
    }

    counter = 0;
    float *texels = new float[shape->getTexels().size() * 2];
    for(int i = 0; i < shape->getTexels().size(); ++i) {
      texels[counter++] = shape->getTexels()[i].x;
      texels[counter++] = shape->getTexels()[i].y;
    }

    /*const GLubyte *indices = shape->getIndices().data();
    GLubyte *indicesPtr = new GLubyte[shape->getIndices().size()];

    for(int i = 0; i < shape->getIndices().size(); i++) {
      indicesPtr[i] = indices[i];
    }*/

    // Enable client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texels);

    // Deference
    glDrawElements(GL_TRIANGLES, shape->getIndices().size(), 
                   GL_UNSIGNED_BYTE, shape->getIndices().data());

    // Disable client states
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    delete[] vertices;
    delete[] normals;
    delete[] texels;
  }

}