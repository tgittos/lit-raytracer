#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "Intersection.hpp"
#include "Ray.hpp"

class Intersectable {
  public:
  Intersectable(){};
  ~Intersectable(){};

  virtual Intersection* Intersect(Ray ray, float t_max) = 0;
};

#endif
