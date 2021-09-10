#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Intersection.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"

class Shape {
  public:
  Shape():_bb(NULL){};
  ~Shape(){ delete _bb; };

  void SetTransform(glm::mat4 value) { _transform = value; _inverseTransform = glm::inverse(value); }
  virtual BoundingBox GetBoundingBox() = 0;

  virtual Intersection* Intersects(const Ray ray, float t_max) = 0;

  protected:
  glm::mat4 _transform;
  glm::mat4 _inverseTransform;
  BoundingBox* _bb;
};

#endif
