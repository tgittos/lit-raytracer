#include <iostream>
#include "TriMesh.hpp"

void TriMesh::AddVertex(float x, float y, float z) {
  _vertices.push_back(glm::vec3(x, y, z));
  glm::vec3 min = _vertices[0];
  glm::vec3 max = _vertices[0];
  for (int i = 0; i < _vertices.size(); ++i) {
    if (_vertices[i][0] < min[0]) { min[0] = _vertices[i][0]; }
    if (_vertices[i][1] < min[1]) { min[1] = _vertices[i][1]; }
    if (_vertices[i][2] < min[2]) { min[2] = _vertices[i][2]; }
    if (_vertices[i][0] > max[0]) { max[0] = _vertices[i][0]; }
    if (_vertices[i][1] > max[1]) { max[1] = _vertices[i][1]; }
    if (_vertices[i][2] > max[2]) { max[2] = _vertices[i][2]; }
    _bb = new BoundingBox(min, max);
  }
}

void TriMesh::AddTriangle(int v1, int v2, int v3) {
  _triangles.push_back(glm::vec3(v1, v2, v3));
}

Intersection* TriMesh::Intersects(Ray ray, float t_max) {
  glm::vec4 homoRayOrigin = glm::vec4(ray.GetOrigin(), 1.f) * _inverseTransform;
  glm::vec4 homoRayDirection = glm::vec4(ray.GetDirection(), 0.f) * _inverseTransform;
  glm::vec3 rayOrigin = glm::vec3(homoRayOrigin[0], homoRayOrigin[1], homoRayOrigin[2]);
  glm::vec3 rayDirection = glm::vec3(homoRayDirection[0], homoRayDirection[1], homoRayDirection[2]);
  if (!_bb->Intersect(rayOrigin, rayDirection, t_max)) { return NULL; }
  Intersection* intersection = NULL;
  for(std::vector<glm::vec3>::iterator itr = _triangles.begin(); itr != _triangles.end(); ++itr) {
    glm::vec3 tri = *itr;
    glm::vec3 v1 = _vertices[tri[0]];
    glm::vec3 v2 = _vertices[tri[1]];
    glm::vec3 v3 = _vertices[tri[2]];
    
    glm::vec3 norm = glm::normalize(glm::cross(v2 - v1, v3 - v1));
    if (glm::dot(rayDirection, norm) == 0) { break; }
    float t = glm::dot(v1 - rayOrigin, norm) / glm::dot(rayDirection, norm);
    if (t < EPSILON || t > t_max) { break; }
    //Debug details of triangle.
    glm::vec3 intersectionPoint = rayOrigin + (rayDirection * t);
    if (
      glm::dot(glm::cross(v2 - v1, intersectionPoint - v1), norm) >= 0.f &&
      glm::dot(glm::cross(v3 - v2, intersectionPoint - v2), norm) >= 0.f &&
      glm::dot(glm::cross(v1 - v3, intersectionPoint - v3), norm) >= 0.f
    ) {
      glm::vec4 homoIntersectionPoint = glm::vec4(intersectionPoint, 1.f) * _transform;
      intersectionPoint = glm::vec3(homoIntersectionPoint[0], homoIntersectionPoint[1], homoIntersectionPoint[2]);
      glm::vec4 homoNorm = glm::vec4(norm, 0.f) * glm::transpose(_inverseTransform);
      norm = glm::normalize(glm::vec3(homoNorm[0], homoNorm[1], homoNorm[2]));
      intersection = new Intersection(ray, intersectionPoint, t, norm);
      //std::cout << "\tfound valid intersection at t: " << t << std::endl;
      break;
    } else {
      //std::cout << "\tintersection on plane, but not inside triangle. intersection at t: " << t << std::endl;
    }
  }
  return intersection;
}

BoundingBox TriMesh::GetBoundingBox() {
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
