#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderObject.hpp"
#include "BoundingBox.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

class OctreeNode {
  public:
  OctreeNode(glm::vec3 min, glm::vec3 max);
  ~OctreeNode();

  BoundingBox* GetBoundingBox() { return _bb; }

  Intersection* Intersects(Ray ray, float t_max);

  void Insert(RenderObject* object);

  int GetNodeCounts() { return _objects.size(); }

  private:
  BoundingBox* _bb;
  std::vector<RenderObject*> _objects;
};

class Octree {
  public:
  Octree(std::vector<RenderObject*> objects, glm::vec3 min, glm::vec3 max);
  ~Octree(){};

  Intersection* Intersects(Ray ray, float t_max);

  private:
  std::vector<OctreeNode> _nodes;

  bool BoundingBoxOverlaps(glm::vec3 aMin, glm::vec3 aMax, glm::vec3 bMin, glm::vec3 bMax);
};

#endif
