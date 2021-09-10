#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Ray;
class Intersection;

class Light {
  public:
  Light(float x, float y, float z, float r, float g, float b, bool isDirectional);
  ~Light(){};

  glm::vec3 GetLocation() { return _location; }
  glm::vec3 GetColor() { return _color; }
  bool IsDirectional() { return _isDirectional; }

  Ray GetRayToLight(Intersection intersection);

  float GetTToLight(Ray ray);

  private:
  glm::vec3 _location;
  glm::vec3 _color;
  bool _isDirectional;
};

#endif
