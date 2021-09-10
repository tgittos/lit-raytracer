#include <iostream>
#include "Camera.hpp"
#include "Transform.hpp"

Camera::Camera(int posX, int posY, int posZ,
       int lookAtX, int lookAtY, int lookAtZ,
       int upX, int upY, int upZ,
       int fovy)
  : _fovy(fovy) {
  _position = glm::vec3(posX, posY, posZ);
  glm::vec3 lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
  glm::vec3 up = glm::normalize(glm::vec3(upX, upY, upZ));
  _w = glm::normalize(_position - lookAt);
  _u = glm::normalize(glm::cross(up, _w));
  _v = glm::cross(_w, _u);
  //Debug camera basis.
}

Ray Camera::GetRayForPixel(int x, int y, int width, int height) {
  float fixedX = x + 0.5;
  float fixedY = y + 0.5;
  float pixelNormDeltaX = (fixedX - (width * 1.f / 2.f)) / (width * 1.f / 2.f);
  float pixelNormDeltaY = ((height * 1.f / 2.f) - fixedY) / (height * 1.f / 2.f);
  float tanfovy = tan((_fovy * 1.f * Transform::PI / 180) / 2);
  float alpha = (tanfovy * width * 1.f / height) * pixelNormDeltaX;
  float beta = tanfovy * pixelNormDeltaY;
  //std::cout << "alpha:\t" << alpha << std::endl;
  //std::cout << "beta:\t" << beta << std::endl;
  return Ray(_position, glm::normalize((alpha * _u) + (beta * _v) - _w));
}
