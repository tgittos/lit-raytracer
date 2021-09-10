#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderObject.hpp"
#include "Shape.hpp"
#include "Material.hpp"
#include "BoundingBox.hpp"

class Primitive : public RenderObject {
  public:
  Primitive(Shape* geometry, Material* material);
  ~Primitive();

  Intersection* Intersects(const Ray ray, float t_max);
  glm::vec3 GetColor(std::vector<Light*> lights, Intersection* intersection, glm::vec3 reflectColor);
  Material* GetMaterial() { return _material; }
  BoundingBox GetBoundingBox() { return _geometry->GetBoundingBox(); }

  private:
  Shape* _geometry;
  Material* _material;
};

#endif
