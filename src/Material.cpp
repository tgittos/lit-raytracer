#include <iostream>
#include <algorithm>
#include "Material.hpp"

Material::Material():
  _attenuation(glm::vec3(1.f, 0.f, 0.f)) {}

void Material::SetAmbient(float r, float g, float b) {
  _ambient = glm::vec3(r, g, b);
}

void Material::SetDiffuse(float r, float g, float b) {
  _diffuse = glm::vec3(r, g, b);
}

void Material::SetSpecular(float r, float g, float b) {
  _specular = glm::vec3(r, g, b);
}

void Material::SetEmission(float r, float g, float b) {
  _emission = glm::vec3(r, g, b);
}

void Material::SetAttenuation(float a, float b, float c) {
  _attenuation = glm::vec3(a, b, c);
}

glm::vec3 Material::GetColor(std::vector<Light*> lights, Intersection* intersection, glm::vec3 reflectColor) {
  glm::vec3 color = _ambient + _emission;
  for (std::vector<Light*>::iterator itr = lights.begin(); itr != lights.end(); ++itr) {
    Light* light = *itr;
    float d = glm::length(light->GetLocation() - intersection->GetPoint());
    float attenuation = 1.f / (_attenuation[0] + _attenuation[1] * d + _attenuation[2] * d * d);
    glm::vec3 n = intersection->GetNormal();
    glm::vec3 l = glm::normalize(light->GetRayToLight(*intersection).GetDirection());
    glm::vec3 h = glm::normalize(l + -intersection->GetRay().GetDirection());
    float ndotl = glm::dot(n, l);
    float ndoth = glm::dot(n, h);
    glm::vec3 lambert = _diffuse * std::max(0.f, ndotl);
    glm::vec3 phong = _specular * std::pow(std::max(0.f, ndoth), _shininess);
    color += light->GetColor() * attenuation * (lambert + phong);
    //Debug material.
  }
  color += _specular * reflectColor;
  for (int i = 0; i < 3; ++i) { color[i] = std::min(1.f, color[i]); }
  return color;
}
