#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.hpp"

class Renderer {
  public:
  Renderer(){};
  ~Renderer(){};

  void static RenderToImage(Scene* scene);
};

#endif
