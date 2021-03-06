#include <algorithm>
#include <iostream>
#include "Sphere.hpp"

Sphere::Sphere(float x, float y, float z, float radius)
  :_x(x), _y(y), _z(z), _radius(radius) {
  _bb = new BoundingBox(glm::vec3(x - radius, y - radius, z - radius), glm::vec3(x + radius, y + radius, z + radius));
}

Intersection* Sphere::Intersects(Ray ray, float t_max) {
  glm::vec3 sphereCenter = glm::vec3(_x, _y, _z);
  glm::vec4 homoRayDir = glm::vec4(ray.GetDirection(), 0.f) * _inverseTransform;
  glm::vec4 homoRayOrigin = glm::vec4(ray.GetOrigin(), 1.f) * _inverseTransform;
  glm::vec3 rayDir = glm::vec3(homoRayDir[0], homoRayDir[1], homoRayDir[2]);
  glm::vec3 rayOrigin = glm::vec3(homoRayOrigin[0], homoRayOrigin[1], homoRayOrigin[2]);
  glm::vec3 rayDelta = rayOrigin - sphereCenter;
  if (!_bb->Intersect(rayOrigin, rayDir, t_max)) { return NULL; }
  
  //std::cout << "\tsphere center: [" << sphereCenter[0] << ", " << sphereCenter[1] << ", " << sphereCenter[2] << "]" << std::endl;
  //std::cout << "\trayDir: [" << rayDir[0] << ", " << rayDir[1] << ", " << rayDir[2] << "]" << std::endl;
  //std::cout << "\trayOrigin: [" << rayOrigin[0] << ", " << rayOrigin[1] << ", " << rayOrigin[2] << "]" << std::endl;
  //std::cout << "\trayDelta: [" << rayDelta[0] << ", " << rayDelta[1] << ", " << rayDelta[2] << "]" << std::endl;
  float a = glm::dot(rayDir, rayDir);
  float b = 2 * glm::dot(rayDir, rayDelta);
  float c = glm::dot(rayDelta, rayDelta) - (_radius * _radius);
  //std::cout << "\ta: " << a << std::endl;
  //std::cout << "\tb: " << b << std::endl;
  //std::cout << "\tc: " << c << std::endl;
  float descriminate = b * b - (4 * a * c);
  //std::cout << "\tDescriminate: " << descriminate << std::endl;
  if (descriminate < 0.f) { return false; }
  float result1 = (-b-sqrt(descriminate)) / (2 * a);
  float result2 = (-b+sqrt(descriminate)) / (2 * a);
  //std::cout << "\tresult1: " << result1 << std::endl;
  //std::cout << "\tresult2: " << result2 << std::endl;
  float t = 0.f;
  float max_t = std::max(result1, result2);
  float min_t = std::min(result1, result2);
  if (max_t < 0) { return NULL; }
  if (min_t > 0) { t = min_t; }
  else { t = max_t; }
  if (t > t_max) { return NULL; }
  glm::vec3 intersectionPoint = (rayOrigin + (rayDir * t));
  glm::vec3 normal = intersectionPoint - sphereCenter;
  
  glm::vec4 homoIntersectionPoint = glm::vec4(intersectionPoint, 1.f) * _transform;
  intersectionPoint = glm::vec3(homoIntersectionPoint[0], homoIntersectionPoint[1], homoIntersectionPoint[2]);
  
  glm::vec4 homoNormal = glm::vec4(normal, 0.f) * glm::transpose(_inverseTransform);
  normal = glm::normalize(glm::vec3(homoNormal[0], homoNormal[1], homoNormal[2]));
  
  /*
  std::cout << "[" << _transform[0][0] << ", " << _transform[0][1] << ", " << _transform[0][2] << ", " << _transform[0][3] << std::endl
    << _transform[1][0] << ", " << _transform[1][1] << ", " << _transform[1][2] << ", " << _transform[1][3] << std::endl
    << _transform[2][0] << ", " << _transform[2][1] << ", " << _transform[2][2] << "," << _transform[2][3] << std::endl
    << _transform[3][0] << ", " << _transform[3][1] << ", " << _transform[3][2] << ", " << _transform[3][3] << "]" << std::endl;
  */
  glm::vec4 transformedCenter = glm::vec4(sphereCenter, 1.f) * _transform;
  //std::cout << "This object's location: [" << transformedCenter[0] << ", " << transformedCenter[1] << ", " << transformedCenter[2] << "]" << std::endl;
  
  return new Intersection(ray, intersectionPoint, t, normal);
}

BoundingBox Sphere::GetBoundingBox() {
  glm::vec4 min = glm::vec4(_bb->GetMin(), 1.f);
  glm::vec4 max = glm::vec4(_bb->GetMax(), 1.f);
  glm::vec4 tMin = min * _transform;
  glm::vec4 tMax = max * _transform;
  glm::vec3 finalMin = glm::vec3(
    std::min(tMin[0], tMax[0]), std::min(tMin[1], tMax[1]), std::min(tMin[2], tMax[2])
  );
  glm::vec3 finalMax = glm::vec3(
    std::max(tMin[0], tMax[0]), std::max(tMin[1], tMax[1]), std::max(tMin[2], tMax[2])
  );
  return BoundingBox(glm::vec3(finalMin[0], finalMin[1], finalMin[2]), glm::vec3(finalMax[0], finalMax[1], finalMax[2]));
}
