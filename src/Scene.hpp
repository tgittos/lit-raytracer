#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <map>
#include <queue>
#include <vector>

#include "Image.hpp"
#include "Camera.hpp"
#include "RenderObject.hpp"
#include "Light.hpp"
#include "TriMesh.hpp"
#include "Material.hpp"
#include "Octree.hpp"

class Scene {
  public:
  Scene();
  ~Scene();

  void AddRenderObject(RenderObject* renderObject);
  void AddLight(Light* light);
  void SetCamera(Camera* value) { _camera = value; }
  void SetOutputFilename(std::string value) { _outputFilename = value; }
  void SetWidth(int value) { _width = value; }
  void SetHeight(int value) { _height = value; }
  void SetMaxDepth(int value) { _maxDepth = value; }
  void SetMin(glm::vec3 value) { _min = value; }
  void SetMax(glm::vec3 value) { _max = value; }

  void RenderToImage();

  private:
  int _width;
  int _height;
  std::string _outputFilename;
  int _maxDepth;
  Image* _img;
  Camera* _camera;
  std::vector<RenderObject*> _renderObjects;
  std::vector<Light*> _lights;
  glm::vec3 _min;
  glm::vec3 _max;
  Octree* _octree;

  glm::vec3 TraceRay(Ray ray, int depth = 0);
  Intersection* Intersect(Ray ray, float max_t = 100.f, bool lighting = false);
};

#endif
