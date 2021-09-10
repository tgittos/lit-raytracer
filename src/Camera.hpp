#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Ray.hpp"

class Camera {
  public:
  Camera(){};
  Camera(int posX, int posY, int posZ,
         int lookAtX, int lookAtY, int lookAtZ,
         int upX, int upY, int upZ,
         int fovy);
  ~Camera(){};

  Ray GetRayForPixel(int x, int y, int w, int h);

  private:
  glm::vec3 _position;
  glm::vec3 _u;
  glm::vec3 _v;
  glm::vec3 _w;
  int _fovy;
};

#endif
