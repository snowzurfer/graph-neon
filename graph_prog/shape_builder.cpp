
// Includes
#include <shape_builder.h>
#include <lnfw/physics/Vector3/Vec3.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <app_globals.h>

namespace winapp {

  // TODO Complete function
  ShapeComp *ShapeBuilder::buildCube(unsigned int detail) {
      // Number of divisions for both width and height
      unsigned int divisions = (unsigned int)std::pow(2.0, (int)detail);

      // Length of a chunk
      float chunkLength = 1.f / divisions;

      // Vectors to be loaded with vertices, normals, indices and texels
      std::vector<Vec3> vertices;
      std::vector<Vec3> normals;
      std::vector<Texel> texels;
      std::vector<unsigned int> indices;
      

      // Front face, z stays fixed
      for(float y = 1; y >= 1.f; y -= chunkLength) {
        for(float x = 0; x <= 1.f; x += chunkLength) {
          vertices.push_back(Vec3(x - 0.5f, y - 0.5f , 0.5f));
          normals.push_back(Vec3(0.f, 0.f, 1.f));
          texels.push_back(Texel(0.25f + (x / 4.f), 0.25f + ((1 - y) / 4.f)));
        }
      }
      /*// Set indices
      int indicesNum = 3 * std::pow(2.f, (int)divisions + 1);
      int column = 0;

      for(int row = 0; row < divisions; ++row) {
        for (int column = row * )
        {
        }
      }

      indices.push_back() */


      return new ShapeComp(indices, vertices, normals, texels);
  }

  ShapeComp * ShapeBuilder::buildDisk(unsigned int detail) {
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
    std::vector<unsigned int> indices;

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
      indices.push_back(i + 2);

      // Increase angle
      angle += angleStep;
    }

    return new ShapeComp(indices, vertices, normals, texels);
  }

}
// EO Namespace