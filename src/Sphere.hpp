#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.hpp"

class Sphere : public Shape {
  public:
  Sphere(float x, float y, float z, float radius);
  ~Sphere(){};

  BoundingBox GetBoundingBox();

  Intersection* Intersects(const Ray ray, float t_max);

  private:
  float _x, _y, _z, _radius;
};

#endif
