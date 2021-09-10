#include <iostream>
#include "Primitive.hpp"

Primitive::Primitive(Shape* geometry, Material* material)
  :_geometry(geometry), _material(material) {}

Primitive::~Primitive() {
  delete _geometry;
  delete _material;
}

Intersection* Primitive::Intersects(const Ray ray, float t_max) {
  return _geometry->Intersects(ray, t_max);
}

glm::vec3 Primitive::GetColor(std::vector<Light*> lights, Intersection* intersection, glm::vec3 reflectColor) {
  return _material->GetColor(lights, intersection, reflectColor);
}
