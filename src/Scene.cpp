#include "Scene.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include <time.h>

#include "Ray.hpp"
#include "RenderObject.hpp"
#include "Primitive.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "Intersection.hpp"

Scene::Scene()
  :_width(100), _height(100), _outputFilename("output.png"), _maxDepth(5) {}

Scene::~Scene() {
  for(std::vector<RenderObject*>::iterator itr = _renderObjects.begin(); itr != _renderObjects.end(); ++itr) {
    delete *itr;
  }
  for(std::vector<Light*>::iterator itr = _lights.begin(); itr != _lights.end(); ++itr) {
    delete *itr;
  }
  delete _img;
  delete _camera;
  delete _octree;
}

void Scene::RenderToImage() {
  std::cout << "Building octree..." << std::endl;
  _octree = new Octree(_renderObjects, _min, _max);
  std::cout << "Starting render..." << std::endl;
  time_t start = std::time(NULL);
  _img = new Image(_width, _height);
  #pragma omp parallel for
  for (int y = 0; y < _height; y++) {
    for (int x = 0; x < _width; x++) {
      Ray ray = _camera->GetRayForPixel(x, y, _width, _height);
      glm::vec3 color = TraceRay(ray);
      _img->SetPixelColor(x, y, color);
    }
  }
  //Debug single ray.
  _img->SaveFile(_outputFilename);
  time_t finish = std::time(NULL);
  std::cout << "Render completed in " << (finish - start) << " seconds" << std::endl;
}

glm::vec3 Scene::TraceRay(Ray ray, int depth) {
  glm::vec3 color = glm::vec3(0.f);
  Intersection* intersection = Intersect(ray);
  //std::cout << "Calculating lighting" << std::endl;
  if (NULL != intersection) {
    //std::cout << "\tCasting light from intersection point" << std::endl;
    std::vector<Light*> seenLights;
    for (unsigned int i = 0; i < _lights.size(); ++i) {
      //std::cout << "Sending shadow ray to light " << i << std::endl;
      Ray lightRay = _lights[i]->GetRayToLight(*intersection);
      float max_t = _lights[i]->GetTToLight(lightRay);
      //Debug trace.
      Intersection* lightIntersection = Intersect(lightRay, max_t, false);
      if (NULL == lightIntersection) {
        //std::cout << "\tNothing blocking the light, adding light to list" << std::endl;
        seenLights.push_back(_lights[i]);
      }
    }
    glm::vec3 reflectColor = glm::vec3(0.f);
    //std::cout << "Computing reflections" << std::endl;
    Material* mat = intersection->GetIntersected()->GetMaterial();
    if (mat->GetSpecular() != glm::vec3()) {
      glm::vec3 rayDir = ray.GetDirection();
      glm::vec3 norm = intersection->GetNormal();
      glm::vec3 reflectDir = rayDir - 2 * glm::dot(rayDir, norm) * norm;
      Ray reflection = Ray(intersection->GetPoint() + norm * 0.001f, reflectDir);
      if (depth < _maxDepth) {
        reflectColor = TraceRay(reflection, ++depth);
      }
    }
    color += intersection->GetIntersected()->GetColor(seenLights, intersection, reflectColor);
  }
  return color;
}

Intersection* Scene::Intersect(Ray ray, float max_t, bool lighting) {
  Intersection* intersection = _octree->Intersects(ray, max_t);
  return intersection;
}

void Scene::AddRenderObject(RenderObject* renderObject) {
  _renderObjects.push_back(renderObject);
}

void Scene::AddLight(Light* light) {
  _lights.push_back(light);
}
