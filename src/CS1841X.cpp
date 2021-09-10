#include <queue>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "CS1841X.hpp"
#include "Scene.hpp"
#include "Image.hpp"
#include "Camera.hpp"
#include "RenderObject.hpp"
#include "Light.hpp"
#include "TriMesh.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Primitive.hpp"
#include "Transform.hpp"

CS1841X::CS1841X(std::string filename)
  :FileFormat(filename) {
    InitHandlers();
    _materialBuffer = new Material();
    _meshBuffer = NULL;
    _transforms.push(glm::mat4(1.f));
  }

CS1841X::~CS1841X(){
  for(std::map<const std::string, CommandHandler*>::iterator itr = _handlers.begin(); itr != _handlers.end(); ++itr) {
    delete itr->second;
  }
  delete _meshBuffer;
  delete _materialBuffer;
}

void CS1841X::ReadIntoScene(Scene* scene) {
  _scene = scene;
  std::vector<std::string> lines = ReadFile(_filename);
  for (size_t i = 0; i < lines.size(); i++) {
    std::string line = lines[i];
    // ignore blank lines and comments
    if ((line.find_first_not_of(" \t\r\n") == std::string::npos) ||
        (line[0] == '#')) {
      continue;
    }
    std::queue<std::string> args;
    std::string argDelimiter(" ");
    Split(args, line, argDelimiter);
    std::string cmd = args.front();
    args.pop();
    if (_handlers.count(cmd) == 0) {
      std::cout << "Don't know how to handle command \"" << cmd << "\"" << std::endl;
    } else {
      CommandHandler h = *_handlers[cmd];
      (h.object->*h.handlerPointer)(args);
    }
  }
  if (NULL != _meshBuffer) {
    //std::cout << "Moving mesh buffer to display list" << std::endl;
    _meshBuffer->SetTransform(_transforms.top());
    Primitive* primitive = new Primitive(static_cast<Shape*>(_meshBuffer), new Material(*_materialBuffer));
    _scene->AddRenderObject(static_cast<RenderObject*>(primitive));
    _meshBuffer = NULL;
  }
  _scene->SetMin(_min);
  _scene->SetMax(_max);
}

void CS1841X::InitHandlers(){
  _handlers.insert(std::pair<const std::string, CommandHandler*>("size", new CommandHandler(this, &CS1841X::SizeHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("output", new CommandHandler(this, &CS1841X::OutputHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("camera", new CommandHandler(this, &CS1841X::CameraHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("sphere", new CommandHandler(this, &CS1841X::SphereHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("directional", new CommandHandler(this, &CS1841X::DirectionalHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("point", new CommandHandler(this, &CS1841X::PointHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("vertex", new CommandHandler(this, &CS1841X::VertexHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("tri", new CommandHandler(this, &CS1841X::TriHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("ambient", new CommandHandler(this, &CS1841X::AmbientHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("diffuse", new CommandHandler(this, &CS1841X::DiffuseHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("specular", new CommandHandler(this, &CS1841X::SpecularHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("emission", new CommandHandler(this, &CS1841X::EmissionHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("shininess", new CommandHandler(this, &CS1841X::ShininessHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("pushTransform", new CommandHandler(this, &CS1841X::PushTransformHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("popTransform", new CommandHandler(this, &CS1841X::PopTransformHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("scale", new CommandHandler(this, &CS1841X::ScaleHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("translate", new CommandHandler(this, &CS1841X::TranslateHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("rotate", new CommandHandler(this, &CS1841X::RotateHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("maxdepth", new CommandHandler(this, &CS1841X::MaxDepthHandler)));
  _handlers.insert(std::pair<const std::string, CommandHandler*>("attenuation", new CommandHandler(this, &CS1841X::AttenuationHandler)));
}

void CS1841X::SizeHandler(std::queue<std::string> args) {
  int width, height;
  std::istringstream(args.front()) >> width;
  args.pop();
  std::istringstream(args.front()) >> height;
  _scene->SetWidth(width);
  _scene->SetHeight(height);
}

void CS1841X::OutputHandler(std::queue<std::string>args) {
  _scene->SetOutputFilename(args.front());
}

void CS1841X::CameraHandler(std::queue<std::string> args) {
  int posX, posY, posZ, lookAtX, lookAtY, lookAtZ, upX, upY, upZ, fovy;
  std::istringstream(args.front()) >> posX; args.pop();
  std::istringstream(args.front()) >> posY; args.pop();
  std::istringstream(args.front()) >> posZ; args.pop();
  std::istringstream(args.front()) >> lookAtX; args.pop();
  std::istringstream(args.front()) >> lookAtY; args.pop();
  std::istringstream(args.front()) >> lookAtZ; args.pop();
  std::istringstream(args.front()) >> upX; args.pop();
  std::istringstream(args.front()) >> upY; args.pop();
  std::istringstream(args.front()) >> upZ; args.pop();
  std::istringstream(args.front()) >> fovy;

  _scene->SetCamera(new Camera(posX, posY, posZ,
                       lookAtX, lookAtY, lookAtZ,
                       upX, upY, upZ,
                       fovy));
}

void CS1841X::SphereHandler(std::queue<std::string> args) {
  float x, y, z, radius;
  std::istringstream(args.front()) >> x; args.pop();
  std::istringstream(args.front()) >> y; args.pop();
  std::istringstream(args.front()) >> z; args.pop();
  std::istringstream(args.front()) >> radius;

  Sphere* sphere = new Sphere(x, y, z, radius);

  //std::cout << "Adding sphere at [" << x << ", " << y << ", " << z << "]" << std::endl;
  glm::mat4 transform = _transforms.top();
  /*
  std::cout << "[" << transform[0][0] << ", " << transform[0][1] << ", " << transform[0][2] << ", " << transform[0][3] << std::endl
  << transform[1][0] << ", " << transform[1][1] << ", " << transform[1][2] << ", " << transform[1][3] << std::endl
  << transform[2][0] << ", " << transform[2][1] << ", " << transform[2][2] << "," << transform[2][3] << std::endl
  << transform[3][0] << ", " << transform[3][1] << ", " << transform[3][2] << ", " << transform[3][3] << "]" << std::endl;
  */
  sphere->SetTransform(_transforms.top());
  Primitive* primitive = new Primitive(static_cast<Shape*>(sphere), new Material(*_materialBuffer));

  _scene->AddRenderObject(static_cast<RenderObject*>(primitive));

  BoundingBox bb = sphere->GetBoundingBox();
  glm::vec3 bbMin = bb.GetMin();
  glm::vec3 bbMax = bb.GetMax();
  if (bbMin[0] < _min[0]) { _min[0] = bbMin[0]; }
  if (bbMin[1] < _min[1]) { _min[1] = bbMin[1]; }
  if (bbMin[2] < _min[2]) { _min[2] = bbMin[2]; }
  if (bbMax[0] > _max[0]) { _max[0] = bbMax[0]; }
  if (bbMax[1] > _max[1]) { _max[1] = bbMax[1]; }
  if (bbMax[2] > _max[2]) { _max[2] = bbMax[2]; }
  //Debug min and max update.
}

void CS1841X::VertexHandler(std::queue<std::string> args) {
  float x, y, z;
  std::istringstream(args.front()) >> x; args.pop();
  std::istringstream(args.front()) >> y; args.pop();
  std::istringstream(args.front()) >> z;
  //std::cout << "Adding vertex [" << x << ", " << y << ", " << z << "] to vertex buffer" << std::endl;
  glm::vec3 vertex = glm::vec3(x, y, z);
  _vertexBuffer.push_back(vertex);
}

void CS1841X::AmbientHandler(std::queue<std::string> args) {
  float r, g, b;
  std::istringstream(args.front()) >> r; args.pop();
  std::istringstream(args.front()) >> g; args.pop();
  std::istringstream(args.front()) >> b;
  _materialBuffer->SetAmbient(r, g, b);
}

void CS1841X::DiffuseHandler(std::queue<std::string> args) {
  float r, g, b;
  std::istringstream(args.front()) >> r; args.pop();
  std::istringstream(args.front()) >> g; args.pop();
  std::istringstream(args.front()) >> b;
  _materialBuffer->SetDiffuse(r, g, b);
}

void CS1841X::SpecularHandler(std::queue<std::string> args) {
  float r, g, b;
  std::istringstream(args.front()) >> r; args.pop();
  std::istringstream(args.front()) >> g; args.pop();
  std::istringstream(args.front()) >> b;
  _materialBuffer->SetSpecular(r, g, b);
}

void CS1841X::EmissionHandler(std::queue<std::string> args) {
  float r, g, b;
  std::istringstream(args.front()) >> r; args.pop();
  std::istringstream(args.front()) >> g; args.pop();
  std::istringstream(args.front()) >> b;
  _materialBuffer->SetEmission(r, g, b);
}

void CS1841X::ShininessHandler(std::queue<std::string> args) {
  float shininess;
  std::istringstream(args.front()) >> shininess;
  _materialBuffer->SetShininess(shininess);
}

void CS1841X::TriHandler(std::queue<std::string> args) {
  int v1, v2, v3;
  std::istringstream(args.front()) >> v1; args.pop();
  std::istringstream(args.front()) >> v2; args.pop();
  std::istringstream(args.front()) >> v3;
  _meshBuffer = new TriMesh();
  glm::vec3 vertex1 = _vertexBuffer[v1];
  glm::vec3 vertex2 = _vertexBuffer[v2];
  glm::vec3 vertex3 = _vertexBuffer[v3];
  _meshBuffer->AddVertex(vertex1[0], vertex1[1], vertex1[2]);
  _meshBuffer->AddVertex(vertex2[0], vertex2[1], vertex2[2]);
  _meshBuffer->AddVertex(vertex3[0], vertex3[1], vertex3[2]);
  _meshBuffer->AddTriangle(0, 1, 2);
  //Debug tri command.
  _meshBuffer->SetTransform(_transforms.top());
  Primitive* primitive = new Primitive(static_cast<Shape*>(_meshBuffer), new Material(*_materialBuffer));
  _scene->AddRenderObject(static_cast<RenderObject*>(primitive));
  BoundingBox bb = _meshBuffer->GetBoundingBox();
  glm::vec3 bbMin = bb.GetMin();
  glm::vec3 bbMax = bb.GetMax();
  if (bbMin[0] < _min[0]) { _min[0] = bbMin[0]; }
  if (bbMin[1] < _min[1]) { _min[1] = bbMin[1]; }
  if (bbMin[2] < _min[2]) { _min[2] = bbMin[2]; }
  if (bbMax[0] > _max[0]) { _max[0] = bbMax[0]; }
  if (bbMax[1] > _max[1]) { _max[1] = bbMax[1]; }
  if (bbMax[2] > _max[2]) { _max[2] = bbMax[2]; }
  //Debug min and max update.
  _meshBuffer = NULL;
}

void CS1841X::DirectionalHandler(std::queue<std::string> args) {
  bool isDirectional = true;
  float x = 0.f, y = 0.f, z = 0.f, r = 0.f, g = 0.f, b = 0.f;
  std::istringstream(args.front()) >> x; args.pop();
  std::istringstream(args.front()) >> y; args.pop();
  std::istringstream(args.front()) >> z; args.pop();
  std::istringstream(args.front()) >> r; args.pop();
  std::istringstream(args.front()) >> g; args.pop();
  std::istringstream(args.front()) >> b;
  
  Light* light = new Light(x, y, z, r, g, b, isDirectional);
  
  //std::cout << "Adding light at [" << x << ", " << y << ", " << z << "]" << std::endl;
  
  _scene->AddLight(light);
}

void CS1841X::PointHandler(std::queue<std::string> args) {
  bool isDirectional = false;
  float x = 0.f, y = 0.f, z = 0.f, r = 0.f, g = 0.f, b = 0.f;
  std::istringstream(args.front()) >> x; args.pop();
  std::istringstream(args.front()) >> y; args.pop();
  std::istringstream(args.front()) >> z; args.pop();
  std::istringstream(args.front()) >> r; args.pop();
  std::istringstream(args.front()) >> g; args.pop();
  std::istringstream(args.front()) >> b;
  
  Light* light = new Light(x, y, z, r, g, b, isDirectional);
  
  //std::cout << "Adding light at [" << x << ", " << y << ", " << z << "]" << std::endl;
  
  _scene->AddLight(light);
}

void CS1841X::PushTransformHandler(std::queue<std::string> args) {
  _transforms.push(glm::mat4(_transforms.top()));
}

void CS1841X::PopTransformHandler(std::queue<std::string> args) {
  _transforms.pop();
}

void CS1841X::ScaleHandler(std::queue<std::string> args) {
  float x, y, z;
  std::istringstream(args.front()) >> x; args.pop();
  std::istringstream(args.front()) >> y; args.pop();
  std::istringstream(args.front()) >> z;
  glm::mat4 scale = Transform::scale(x, y, z);
  glm::mat4& transform = _transforms.top();
  transform = scale * transform;
}

void CS1841X::TranslateHandler(std::queue<std::string> args) {
  float x, y, z;
  std::istringstream(args.front()) >> x; args.pop();
  std::istringstream(args.front()) >> y; args.pop();
  std::istringstream(args.front()) >> z;
  glm::mat4& transform = _transforms.top();
  glm::mat4 translate = Transform::translate(x, y, z);
  transform = translate * transform;
}

void CS1841X::RotateHandler(std::queue<std::string> args) {
  float degrees, x, y, z;
  std::istringstream(args.front()) >> x; args.pop();
  std::istringstream(args.front()) >> y; args.pop();
  std::istringstream(args.front()) >> z; args.pop();
  std::istringstream(args.front()) >> degrees;
  glm::mat3 rotation = Transform::rotate(degrees, glm::vec3(x, y, z));
  glm::mat4 homoRotation = glm::mat4(rotation);
  glm::mat4& transform = _transforms.top();
  transform = homoRotation * transform;
}

void CS1841X::MaxDepthHandler(std::queue<std::string> args) {
  int depth;
  std::istringstream(args.front()) >> depth;
  _scene->SetMaxDepth(depth);
}

void CS1841X::AttenuationHandler(std::queue<std::string> args) {
  float a = 0.f, b = 0.f, c = 0.f;
  std::istringstream(args.front()) >> a; args.pop();
  std::istringstream(args.front()) >> b; args.pop();
  std::istringstream(args.front()) >> c;
  _materialBuffer->SetAttenuation(a, b, c);
}
