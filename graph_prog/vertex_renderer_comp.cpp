
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
    
    // Setup rendering; performs type-of-renderer-agnostic steps
    setupRendering(transform, shape, texture, material);
    
    
    //////////RENDER

    
    // Specify data for the arrays

    float *vertices = new float[shape->getVertices().size() * 3];

    int counter = 0;
    for(unsigned int i = 0; i < shape->getVertices().size(); ++i) {
      vertices[counter++] = shape->getVertices()[i].getX();
      vertices[counter++] = shape->getVertices()[i].getY();
      vertices[counter++] = shape->getVertices()[i].getZ();
    }

    counter = 0;
    float *normals = new float[shape->getNormals().size() * 3];
    for(unsigned int i = 0; i < shape->getNormals().size(); ++i) {
      normals[counter++] = shape->getNormals()[i].getX();
      normals[counter++] = shape->getNormals()[i].getY();
      normals[counter++] = shape->getNormals()[i].getZ();
    }

    counter = 0;
    float *texels = new float[shape->getTexels().size() * 2];
    for(unsigned int i = 0; i < shape->getTexels().size(); ++i) {
      texels[counter++] = shape->getTexels()[i].x;
      texels[counter++] = shape->getTexels()[i].y;
    }

    /*const GLushort *indices = shape->getIndices().data();
    GLushort *indicesPtr = new GLushort[shape->getIndices().size()];

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
                   GL_UNSIGNED_SHORT, shape->getIndices().data());

    // Disable client states
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Deactivate blending
    glDisable(GL_BLEND);

    delete[] vertices;
    delete[] normals;
    delete[] texels;

    cleanUpTextures(texture);
  }

}