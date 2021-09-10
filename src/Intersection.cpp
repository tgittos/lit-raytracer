#include "Intersection.hpp"

Intersection::Intersection(Ray ray, glm::vec3 point, float t, glm::vec3 normal)
  :_point(point), _t(t), _normal(normal), _ray(ray) {}
