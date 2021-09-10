#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Intersection.hpp"
#include "Ray.hpp"

class Ray;
class Intersection;
class Light;
class Material;
class BoundingBox;

class RenderObject {
  public:
  RenderObject(){};
  ~RenderObject(){};

  virtual Intersection* Intersects(const Ray ray, float t_max) = 0;
  virtual glm::vec3 GetColor(std::vector<Light*> lights, Intersection* intersection, glm::vec3 reflectColor) = 0;
  virtual Material* GetMaterial() = 0;
  virtual BoundingBox GetBoundingBox() = 0;
};

#endif
