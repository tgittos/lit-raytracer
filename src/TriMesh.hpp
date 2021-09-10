#ifndef TRI_MESH_H
#define TRI_MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shape.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

class TriMesh : public Shape{
  public:
  TriMesh(){};
  ~TriMesh(){};

  BoundingBox GetBoundingBox();

  void AddVertex(float x, float y, float z);
  void AddTriangle(int v1, int v2, int v3);
  Intersection* Intersects(Ray ray, float t_max);

  private:
  static const float EPSILON = 0.0001f;
  std::vector<glm::vec3> _vertices;
  std::vector<glm::vec3> _triangles;
};

#endif
