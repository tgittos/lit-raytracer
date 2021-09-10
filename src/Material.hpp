#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Intersection.hpp"
#include "Light.hpp"

class Material {
  public:
  Material();
  ~Material(){};

  void SetAmbient(float r, float g, float b);
  glm::vec3 GetAmbient() { return _ambient; }
  
  void SetDiffuse(float r, float g, float b);
  glm::vec3 GetDiffuse() { return _diffuse; }
  
  void SetSpecular(float r, float g, float b);
  glm::vec3 GetSpecular() { return _specular; }
  
  void SetEmission(float r, float g, float b);
  glm::vec3 GetEmission() { return _emission; }
  
  void SetShininess(float value) { _shininess = value; }
  float GetShininess() { return _shininess; }
  
  void SetAttenuation(float a, float b, float c);

  glm::vec3 GetColor(std::vector<Light*> light, Intersection* intersection, glm::vec3 reflectColor);

  private:
  glm::vec3 _ambient;
  glm::vec3 _diffuse;
  glm::vec3 _specular;
  glm::vec3 _emission;
  float _shininess;
  glm::vec3 _attenuation;
};

#endif
