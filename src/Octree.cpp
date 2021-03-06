#include <iostream>

#include "Octree.hpp"

Octree::Octree(std::vector<RenderObject*> objects, glm::vec3 min, glm::vec3 max) {
  glm::vec3 delta = max - min;
  glm::vec3 bounds = glm::vec3(delta[0] / 2, delta[1] / 2, delta[2] / 2);
  glm::vec3 origin = max - bounds;
  // back bottom left [0]
  _nodes.push_back(
    OctreeNode(min, origin)
  );
  // back bottom right [1]
  _nodes.push_back(
    OctreeNode(min + glm::vec3(bounds[0], 0, 0) , origin + glm::vec3(bounds[0], 0, 0))
  );
  // front bottom right [2]
  _nodes.push_back(
    OctreeNode(origin - glm::vec3(0, bounds[1], 0), origin + glm::vec3(bounds[0], 0, bounds[2]))
  );
  // front bottom left [3]
  _nodes.push_back(
    OctreeNode(min + glm::vec3(0, 0, bounds[2]), origin + glm::vec3(0, 0, bounds[2]))
  );
  // back top left [4]
  _nodes.push_back(
    OctreeNode(origin - glm::vec3(bounds[0], 0, bounds[2]), origin + glm::vec3(0, bounds[1], 0))
  );
  // back top right [5]
  _nodes.push_back(
    OctreeNode(origin - glm::vec3(0, 0, bounds[2]), max - glm::vec3(0, 0, bounds[2]))
  );
  //front top right [6]
  _nodes.push_back(
    OctreeNode(origin, max)
  );
  // front top left [7]
  _nodes.push_back(
    OctreeNode(origin - glm::vec3(bounds[0], 0, 0), max - glm::vec3(bounds[1], 0, 0))
  );
  for (int i = 0; i < objects.size(); i++) {
    BoundingBox bb = objects[i]->GetBoundingBox();
    glm::vec3 boundingMin = bb.GetMin();
    glm::vec3 boundingMax = bb.GetMax();

    // insert object into any octants where any part of the bb
    // falls in this octant.
    bool inserted = false;
    for (int j = 0; j < _nodes.size(); ++j) {
      BoundingBox* nodeBB = _nodes[j].GetBoundingBox();
      glm::vec3 nodeMin = nodeBB->GetMin();
      glm::vec3 nodeMax = nodeBB->GetMax();
      if (BoundingBoxOverlaps(boundingMin, boundingMax, nodeMin, nodeMax)) {
        _nodes[j].Insert(objects[i]);
        inserted = true;
      }
    }
    if (!inserted) {
      std::cout << "object " << i << " was not inserted into any node" << std::endl;
      std::cout << "objects bounding box:" << std::endl;
      std::cout << "\tmin: [" << boundingMin[0] << ", " << boundingMin[1] << ", " << boundingMin[2] << "]" << std::endl;
      std::cout << "\tmax: [" << boundingMax[0] << ", " << boundingMax[1] << ", " << boundingMax[2] << "]" << std::endl;
    }
  }
  //Debug octree nodes.
}

Intersection* Octree::Intersects(Ray ray, float t_max) {
  Intersection* intersection = NULL;
  for (int i = 0; i < 8; ++i) {
    //std::cout << "Testing ray against octree node " << i << std::endl;
    Intersection* tempIntersection = _nodes[i].Intersects(ray, t_max);
    if (NULL != tempIntersection) {
      if (NULL == intersection || tempIntersection->GetT() < intersection->GetT()) {
        intersection = tempIntersection;
      }
    }
  }
  return intersection;
}

bool Octree::BoundingBoxOverlaps(glm::vec3 aMin, glm::vec3 aMax, glm::vec3 bMin, glm::vec3 bMax) {
  if (aMax[0] < bMin[0]) { return false; } // a is left of b
  if (aMin[0] > bMax[0]) { return false; } // a is right of b
  if (aMax[1] < bMin[1]) { return false; } // a is above b
  if (aMin[1] > bMax[1]) { return false; } // a is below b
  if (aMax[2] < bMin[2]) { return false; } // a is behind b
  if (aMin[2] > bMax[2]) { return false; } // a is in front of b
  return true;
}

OctreeNode::OctreeNode(glm::vec3 min, glm::vec3 max) {
  _bb = new BoundingBox(min, max);
}

OctreeNode::~OctreeNode() {
  //delete _bb;
}

Intersection* OctreeNode::Intersects(Ray ray, float t_max) {
  if (!_bb->Intersect(ray.GetOrigin(), ray.GetDirection(), t_max)) {
    //std::cout << "Ray didn't intersect with node bounding box" << std::endl;
    return NULL;
  }
  Intersection* intersection = NULL;
  //std::cout << "Testing ray against objects in node" << std::endl;
  //std::cout << "Node has " << _objects.size() << " objects" << std::endl;
  for(int i = 0; i < _objects.size(); ++i) {
    Intersection* tempIntersection = _objects[i]->Intersects(ray, t_max);
    if (NULL != tempIntersection) {
      if (NULL == intersection || tempIntersection->GetT() < intersection->GetT()) {
        intersection = tempIntersection;
        intersection->SetIntersected(_objects[i]);
      }
    }
  }
  //if (NULL != intersection) { std::cout << "Found intersection with object" << std::endl; }
  return intersection;
}

void OctreeNode::Insert(RenderObject* object) {
  _objects.push_back(object);
}
