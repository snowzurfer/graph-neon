
// Includes
#include <models_loader.h>
#include <tools/TokenStream.h>
#include <fstream>
#include <string>
#include <vector>
#include <lnfw/physics/Vector3/Vec3.h>
#include <tools/face.h>


namespace winapp {

  ShapeComp *ModelsLoader::load(const char *filename) {
    // Stream for reading the model file
    std::ifstream iFileStream;
    int fileSize = 0;

    // Open the model file
    iFileStream.open(filename, std::ifstream::in);

    // If the file couldn't be opened
    if(iFileStream.is_open() == false) {
      // Report and exit
      return NULL;
    }

    // Move to the end of the file
    iFileStream.seekg( 0, std::ios::end );
    // Read the number of bytes and cast it to integer
    fileSize = ( int )iFileStream.tellg( );
    // Move the pointer back to the beginning of the file
    iFileStream.seekg( 0, std::ios::beg );

    // If the size of the file is corrupted
    if( fileSize <= 0 ) {
      // Report and exit
      return NULL;
    }

    // Create a buffer to hold the data from the file
    char *buffer = new char[fileSize];
    // If the requested memory couldn't be allocated
    if(buffer == 0) {
      // Report and exit
      return NULL;
    }

    // Clear the buffer with end of line chars
    memset( buffer, '\0', fileSize );

    // Create token stream objects for parsing the file
    TokenStream tokenStream, lineStream, faceStream;
    // Create strings used temporary when parsing the file
    std::string tempLine, token;

    // Copy the contents of the file into the chars buffer
    iFileStream.read(buffer, fileSize);
    // Pass the buffer to the tokeniser
    tokenStream.SetTokenStream(buffer);

    // Clear the local buffer since the tokeniser now has a private copy
    delete[] buffer;

    // Create temporary variables to hold the data read from the file
    float tempx, tempy, tempz;
    std::vector<Vec3> verts, norms, texC;
    std::vector<int> faces;

    // Parse the file
    char lineDelimiters[2] = { '\n', ' ' };

    while( tokenStream.MoveToNextLine( &tempLine ) )
    {
      lineStream.SetTokenStream( ( char* )tempLine.c_str( ) );
      tokenStream.GetNextToken( 0, 0, 0 );

      if( !lineStream.GetNextToken( &token, lineDelimiters, 2 ) )
        continue;

      if( strcmp( token.c_str( ), "v" ) == 0 )
      {
        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempx = (float)atof(token.c_str());

        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempy = (float)atof(token.c_str());

        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempz = (float)atof(token.c_str());

        verts.push_back(Vec3(tempx, tempy, tempz));
      }
      else if( strcmp( token.c_str( ), "vn" ) == 0 )
      {
        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempx = (float)atof(token.c_str());

        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempy = (float)atof(token.c_str());

        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempz = (float)atof(token.c_str());

        norms.push_back(Vec3(tempx, tempy, tempz));
      }
      else if( strcmp( token.c_str( ), "vt" ) == 0 )
      {
        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempx = (float)atof(token.c_str());

        lineStream.GetNextToken( &token, lineDelimiters, 2 );
        tempy = (float)atof(token.c_str());

        texC.push_back(Vec3(tempx, tempy, 0));
      }
      else if( strcmp( token.c_str( ), "f" ) == 0 )
      {
        char faceTokens[3] = { '\n', ' ', '/' };
        std::string faceIndex;

        faceStream.SetTokenStream( ( char* )tempLine.c_str( ) );
        faceStream.GetNextToken( 0, 0, 0 );

        for( int i = 0; i < 3; i++ )
        {
          faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
          faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

          faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
          faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

          faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
          faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );
        }
      }
      else if( strcmp( token.c_str( ), "#" ) == 0 )
      {
        //skip
      }

      token[0] = '\0';
    }

    // "Unroll" the loaded obj information into a list of triangles.

    int numFacesElements = (int)faces.size();
    std::vector<Vec3> newVerts, newNorms;
    std::vector<Texel> newTex;
    std::vector<GLuint> indices;
    std::vector<Face> shapeFaces;

    
    for(int i = 0; i < numFacesElements; i += 3) {
      newVerts.push_back(verts[faces[i] - 1]);
      Texel texel(texC[faces[i + 1] - 1].getX(), texC[faces[i + 1] - 1].getY());
      newTex.push_back(texel);
      newNorms.push_back(norms[faces[i + 2] - 1]);
      indices.push_back((GLuint)(i / 3));
    }
    
    shapeFaces = buildFacesVector(indices);


    // Your code must end here, prior to the loaded data being deleted

    verts.clear( );
    norms.clear( );
    texC.clear( );
    faces.clear( );

    return new ShapeComp(indices, newVerts, newNorms, newTex, shapeFaces);
  }

}
// EO Namespace