#ifndef _MODELS_LOADER_H
#define _MODELS_LOADER_H


// Includes
#include <shape_comp.h>


namespace winapp {

  class ModelsLoader
  {
  public:
    // Load the model if possible
    ShapeComp *load(const char *filename);

  private:

  };

}
// EO Namespace

#endif