
// Includes
#include <shapes_factory.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <app_globals.h>

namespace winapp {

  ShapeComp *ShapesFactory::buildCylinder(unsigned int detail) {
    // Number of vertices should be at least 3
    if(detail < 3) {
      std::cout << "Disk Builder Error: not enough vertices! " << detail << std::endl;
      return NULL;
    }

    // Calculate angle step (RADIANS)
    const float angleStep = (2.f * kPi) / (float)detail;

    // Vectors to be loaded with vertices, normals, indices and texels
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Texel> texels;
    std::vector<GLubyte> indices;

    // Store centre of shape
    vertices.push_back(Vec3(0.f, 0.f, 0.f));
    normals.push_back(Vec3(0.f, 0.f, 1.f));
    texels.push_back(Texel(0.5f, 0.5f));

    // Angle increase counter
    float angle = 0.f;

    // For the level of detail
    for(unsigned int i = 0; i < detail; ++i) {
      // Calculate the x and z positions
      float x = std::cosf(angle) / 2;
      float z = std::sinf(angle) / 2;

      // Store data
      vertices.push_back(Vec3(x, 0.f, z));
      normals.push_back(Vec3(0.f, -1.f, 0.f));
      texels.push_back(Texel(x + 0.5f, z + 0.5f));
      indices.push_back(0);
      indices.push_back(i + 1);
      // If at latest vertex
      if(i == (detail - 1)) {
        // Prevent "pac-man" effect
        indices.push_back(1);
      }
      else {
        indices.push_back(i + 2);
      }

      // Increase angle
      angle += angleStep;
    }

    return new ShapeComp(indices, vertices, normals, texels);
  }
  
  ShapeComp *ShapesFactory::buildCube(unsigned int detail) {
      // Number of divisions for both width and height
      unsigned int divisions = (unsigned int)std::pow(2.0, (int)detail);

      // Length of a chunk
      float chunkLength = 1.f / (float)divisions;

      // Vectors to be loaded with vertices, normals, indices and texels
      std::vector<Vec3> vertices;
      std::vector<Vec3> normals;
      std::vector<Texel> texels;
      std::vector<GLubyte> indices;


      // Front face, z stays fixed
      for(float y = 1; y >= 0.f; y -= chunkLength) {
        for(float x = 0; x <= 1.f; x += chunkLength) {
          vertices.push_back(Vec3(x - 0.5f, y - 0.5f , 0.5f));
          normals.push_back(Vec3(0.f, 0.f, 1.f));
          texels.push_back(Texel(0.25f + (x / 4.f), 0.25f + ((1 - y) / 4.f)));
        }
      }

      // Right face, x stays fixed
      for(float y = 1; y >= 0.f; y -= chunkLength) {
        for(float z = 1; z >= 0.f; z -= chunkLength) {
          vertices.push_back(Vec3(0.5f, y - 0.5f , z - 0.5f));
          normals.push_back(Vec3(1.f, 0.f, 0.f));
          texels.push_back(Texel(0.5f + ((z - 1) / 4.f), 0.25f + ((1 - y) / 4.f)));
        }
      }

      // Back face, z stays fixed
      for(float y = 1; y >= 0.f; y -= chunkLength) {
        for(float x = 1; x >= 0.f; x -= chunkLength) {
          vertices.push_back(Vec3(x - 0.5f, y - 0.5f , -0.5f));
          normals.push_back(Vec3(0.f, 0.f, -1.f));
          texels.push_back(Texel(0.75f + ((1 - x) / 4.f), 0.25f + ((1 - y) / 4.f)));
        }
      }

      // Left face, x stays fixed
      for(float y = 1; y >= 0.f; y -= chunkLength) {
        for(float z = 0; z <= 1.f; z += chunkLength) {
          vertices.push_back(Vec3(-0.5f, y - 0.5f , z - 0.5f));
          normals.push_back(Vec3(-1.f, 0.f, 0.f));
          texels.push_back(Texel((z / 4.f), 0.25f + ((1 - y) / 4.f)));
        }
      }

      // Top face, y stays fixed
      for(float z = 0.f; z <= 1.f; z += chunkLength) {
        for(float x = 0.f; x <= 1.f; x += chunkLength) {
          vertices.push_back(Vec3(x - 0.5f, 0.5f , z - 0.5f));
          normals.push_back(Vec3(0.f, 1.f, 0.f));
          texels.push_back(Texel(0.25f + (x / 4.f), (z / 4.f)));
        }
      }

      // Bottom face, y stays fixed
      for(float z = 1.f; z >= 0.f; z -= chunkLength) {
        for(float x = 0.f; x <= 1.f; x += chunkLength) {
          vertices.push_back(Vec3(x - 0.5f, -0.5f , z - 0.5f));
          normals.push_back(Vec3(0.f, -1.f, 0.f));
          texels.push_back(Texel(0.25f + (x / 4.f), 0.5f + ((1 - z) / 4.f)));
        }
      }


      // Rows numbers
      int rowsNum = (int)std::pow((float)(divisions + 1), 2) - (divisions + 1);

      // Indices
      for (int face = 0; face < 6; ++face) {
        // Calculate the starting index for the current face
        int startRow = (int)std::pow((float)(divisions + 1), 2) * face;
        // Calculate the limit of rows to process
        int currentRowsNum = startRow + rowsNum;

        for(int row = startRow; row < currentRowsNum; row += (divisions + 1)) {
          for (unsigned int i = row; i < (row + divisions); i++ ) {
            indices.push_back(i);
            indices.push_back(i + divisions + 1);
            indices.push_back(i + divisions + 2);
            indices.push_back(i);
            indices.push_back(i + divisions + 2);
            indices.push_back(i + 1);
          }
        }
      }

      return new ShapeComp(indices, vertices, normals, texels);
  }

  ShapeComp *ShapesFactory::buildDisk(unsigned int detail) {
    // Number of vertices should be at least 3
    if(detail < 3) {
      std::cout << "Disk Builder Error: not enough vertices! " << detail << std::endl;
      return NULL;
    }

    // Calculate angle step (RADIANS)
    const float angleStep = (2.f * kPi) / (float)detail;

    // Vectors to be loaded with vertices, normals, indices and texels
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Texel> texels;
    std::vector<GLubyte> indices;

    // Store centre of shape
    vertices.push_back(Vec3(0.f, 0.f, 0.f));
    normals.push_back(Vec3(0.f, 0.f, 1.f));
    texels.push_back(Texel(0.5f, 0.5f));

    // Angle increase counter
    float angle = 0.f;

    // For the level of detail
    for(unsigned int i = 0; i < detail; ++i) {
      // Calculate the x and y positions
      float x = std::cosf(angle) / 2;
      float y = std::sinf(angle) / 2;

      // Store data
      vertices.push_back(Vec3(x, y, 0.f));
      normals.push_back(Vec3(0.f, 0.f, 1.f));
      texels.push_back(Texel(x + 0.5f, y + 0.5f));
      indices.push_back(0);
      indices.push_back(i + 1);
      // If at latest vertex
      if(i == (detail - 1)) {
        // Prevent "pac-man" effect
        indices.push_back(1);
      }
      else {
        indices.push_back(i + 2);
      }

      // Increase angle
      angle += angleStep;
    }

    return new ShapeComp(indices, vertices, normals, texels);
  }

  ShapeComp *ShapesFactory::buildPlane(unsigned int detail) {

    // Number of divisions for both width and height
    unsigned int divisions = (unsigned int)std::pow(2.0, (int)detail);

    // Length of a chunk
    float chunkLength = 1.f / (float)divisions;

    // Vectors to be loaded with vertices, normals, indices and texels
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Texel> texels;
    std::vector<GLubyte> indices;


    // Front face, z stays fixed
    for(float y = 1; y >= 0.f; y -= chunkLength) {
      for(float x = 0; x <= 1.f; x += chunkLength) {
        vertices.push_back(Vec3(x - 0.5f, y - 0.5f , 0.f));
        normals.push_back(Vec3(0.f, 0.f, 1.f));
        texels.push_back(Texel(x, (1 - y)));
      }
    }
    // Indices
    int rowsNum = (int)std::pow((float)(divisions + 1), 2) - (divisions + 1);

    for(int row = 0; row < rowsNum; row += (divisions + 1)) {
      for (unsigned int i = row; i < (row + divisions); i++ ) {
        indices.push_back(i);
        indices.push_back(i + divisions + 1);
        indices.push_back(i + divisions + 2);
        indices.push_back(i);
        indices.push_back(i + divisions + 2);
        indices.push_back(i + 1);
      }
    }

    return new ShapeComp(indices, vertices, normals, texels);

  }

  ShapeComp * ShapesFactory::buildCone(unsigned int detail) {
    // Number of vertices should be at least 3
    if(detail < 3) {
      std::cout << "Disk Builder Error: not enough vertices! " << detail << std::endl;
      return NULL;
    }

    // Calculate angle step (RADIANS)
    const float angleStep = (2.f * kPi) / (float)detail;

    // Vectors to be loaded with vertices, normals, indices and texels
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Texel> texels;
    std::vector<GLubyte> indices;

    // Store centre of shape
    vertices.push_back(Vec3(0.f, 0.f, 0.f));
    normals.push_back(Vec3(0.f, -1.f, 0.f));
    texels.push_back(Texel(0.5f, 0.5f));

    // Angle increase counter
    float angle = 0.f;

    // For the level of detail (BASE)
    for(unsigned int i = 0; i < detail; ++i) {
      // Calculate the x and z positions
      float x = std::cosf(angle) / 2;
      float z = std::sinf(angle) / 2;

      // Store data
      vertices.push_back(Vec3(x, 0.f, z));
      normals.push_back(Vec3(0.f, -1.f, 0.f));
      texels.push_back(Texel(x + 0.5f, z + 0.5f));
      indices.push_back(0);
      indices.push_back(i + 1);
      // If at latest vertex
      if(i == (detail - 1)) {
        // Prevent "pac-man" effect
        indices.push_back(1);
      }
      else {
        indices.push_back(i + 2);
      }

      // Increase angle
      angle += angleStep;
    }

    ///////// TOP OF CONE //////////////////////////////////////////////////////////

    angle = 0.f;

    // Vertices before to add vertices of the faces
    int vertexNumStart = vertices.size();
    int vertexNum = vertexNumStart;

    // For each face of the cone
    for (unsigned int i = 0; i < detail; ++i) {
      

      // Top of the cone
      Vec3 topVertex(0.f, 1.f, 0.f);

      // Calculate the x and z positions of the current angle
      float xCurr = std::cosf(angle) / 2;
      float zCurr = -std::sinf(angle) / 2;
      Vec3 currVertex(xCurr, 0.f, zCurr);
      
      Vec3 nextVertex;

      // If at latest vertex
      if(i == (detail - 1)) {
        // Prevent "pac-man" effect
        float xNext = std::cosf(0.f) / 2;
        float zNext = -std::sinf(0.f) / 2;
        nextVertex.set(xNext, 0.f, zNext);
      }
      else {
        // Calculate the x and z positions of the next angle
        float xNext = std::cosf(angle + angleStep) / 2;
        float zNext = -std::sinf(angle + angleStep) / 2;
        nextVertex.set(xNext, 0.f, zNext);
      }

      // Calculate the normals at these points
      Vec3 a = currVertex - topVertex;
      Vec3 b = nextVertex - topVertex;
      Vec3 norm = (b.cross(a));



      // Store data
      vertices.push_back(topVertex);
      vertices.push_back(currVertex);
      vertices.push_back(nextVertex);
      normals.push_back(norm);
      normals.push_back(norm);
      normals.push_back(norm);
      // TODO Change to correct texel coordinates
      texels.push_back(Texel(0.5f, 0.5f));
      texels.push_back(Texel(0.5f, 0.5f));
      texels.push_back(Texel(0.5f, 0.5f));
      indices.push_back(vertexNum++);
      indices.push_back(vertexNum++);
      indices.push_back(vertexNum++);

      // Increase angle
      angle += angleStep;
    }

    return new ShapeComp(indices, vertices, normals, texels);
  }

}
// EO Namespace