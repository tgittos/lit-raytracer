#include <iostream>
#include "Light.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

Light::Light(float x, float y, float z, float r, float g, float b, bool isDirectional)
  :_isDirectional(isDirectional) {
  _location = glm::vec3(x, y, z);
  _color = glm::vec3(r, g, b);
}

Ray Light::GetRayToLight(Intersection intersection) {
  float epsilon = 0.001f;
  glm::vec3 intersectionPoint = intersection.GetPoint();
  glm::vec3 norm = intersection.GetNormal();
  glm::vec3 adjustedPoint = intersectionPoint + (norm * epsilon);
  //Debug ray to light.
  if (_isDirectional) {
    return Ray(adjustedPoint, glm::normalize(_location));
  } else {
    return Ray(adjustedPoint, glm::normalize(_location - adjustedPoint));
  }
}

float Light::GetTToLight(Ray ray) {
  return glm::length(_location - ray.GetOrigin()) / glm::length(ray.GetDirection());
}
