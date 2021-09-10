#include <algorithm>
#include <iostream>

#include "BoundingBox.hpp"

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max)
  :_min(min), _max(max) {}

bool BoundingBox::Intersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, float t_max) {
  //Debug bounding box dimensions.
  float tmin, tmax, tymin, tymax, tzmin, tzmax;
  float divx = 1 / rayDirection[0];
  if (rayDirection[0] >= 0) {
    tmin = (_min[0] - rayOrigin[0]) * divx;
    tmax = (_max[0] - rayOrigin[0]) * divx;
  } else {
    tmin = (_max[0] - rayOrigin[0]) * divx;
    tmax = (_min[0] - rayOrigin[0]) * divx;
  }
  float divy = 1 / rayDirection[1];
  if (rayDirection[1] >= 0) {
    tymin = (_min[1] - rayOrigin[1]) * divy;
    tymax = (_max[1] - rayOrigin[1]) * divy;
  } else {
    tymin = (_max[1] - rayOrigin[1]) * divy;
    tymax = (_min[1] - rayOrigin[1]) * divy;
  }
  //Debug x y components.
  if ((tmin > tymax) || (tymin > tmax)) {
    return false;
  }
  if (tymin > tmin) { tmin = tymin; }
  if (tymax < tmax) { tmax = tymax; }
  float divz = 1 / rayDirection[2];
  if (rayDirection[2] >= 0) {
    tzmin = (_min[2] - rayOrigin[2]) * divz;
    tzmax = (_max[2] - rayOrigin[2]) * divz;
  } else {
    tzmin = (_max[2] - rayOrigin[2]) * divz;
    tzmax = (_min[2] - rayOrigin[2]) * divz;
  }
  //Debug min z components.
  if ((tmin > tzmax) || (tzmin > tmax)) {
    return false;
  }
  if (tzmin > tmin) { tmin = tzmin; }
  if (tzmax < tmax) { tmax = tzmax; }
  //Debug final tmin and tmax.
  float t = 0.f;
  float max_t = std::max(tmin, tmax);
  float min_t = std::min(tmin, tmax);
  if (max_t < 0) { return false; }
  if (min_t > 0) { t = min_t; }
  else { t = max_t; }
  if (t > t_max) { return false; }
  return true;
}
