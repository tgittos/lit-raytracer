#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Ray {
  public:
  Ray(glm::vec3 origin, glm::vec3 direction);
  ~Ray(){};

  glm::vec3 GetOrigin() { return _origin; }
  glm::vec3 GetDirection() { return _direction; }

  private:
  glm::vec3 _origin;
  glm::vec3 _direction;
};

#endif
