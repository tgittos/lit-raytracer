#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Ray.hpp"

class RenderObject;

class Intersection {
  public:
  Intersection(Ray ray, glm::vec3 point, float t, glm::vec3 normal);
  ~Intersection(){};

  glm::vec3 GetPoint() { return _point; }
  glm::vec3 GetNormal() { return _normal; }
  RenderObject* GetIntersected() { return _intersected; }
  Ray GetRay() { return _ray; }
  float GetT() { return _t; }
  void SetIntersected(RenderObject* value) { _intersected = value; }

  private:
  glm::vec3 _point;
  glm::vec3 _normal;
  RenderObject* _intersected;
  Ray _ray;
  float _t;
};

#endif
