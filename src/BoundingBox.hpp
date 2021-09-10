#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray.hpp"

class BoundingBox {
  public:
  BoundingBox(glm::vec3 min, glm::vec3 max);
  ~BoundingBox(){};

  glm::vec3 GetMin() { return _min; }
  glm::vec3 GetMax() { return _max; }

  bool Intersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, float t_max);

  private:
  glm::vec3 _min;
  glm::vec3 _max;
};

#endif
