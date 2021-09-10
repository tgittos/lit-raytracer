#ifndef CS1841_X_H
#define CS1841_X_H

#include <map>
#include <string>
#include <stack>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "FileFormat.hpp"

class TriMesh;
class Material;
class Scene;

class CS1841X : public FileFormat {
  public:
  CS1841X(std::string filename);
  ~CS1841X();

  struct CommandHandler {
    CS1841X* object;
    void (CS1841X::*handlerPointer)(std::queue<std::string>);
  
    CommandHandler(CS1841X* o, void (CS1841X::*h)(std::queue<std::string>))
      : object(o), handlerPointer(h) {}
  };

  void ReadIntoScene(Scene* scene);

  private:
  Scene* _scene;
  std::map<const std::string, CommandHandler*> _handlers;
  TriMesh* _meshBuffer;
  Material* _materialBuffer;
  std::vector<glm::vec3> _vertexBuffer;
  std::stack<glm::mat4> _transforms;
  glm::vec3 _min;
  glm::vec3 _max;

  void InitHandlers();

  void SizeHandler(std::queue<std::string> args);
  void OutputHandler(std::queue<std::string> args);
  void CameraHandler(std::queue<std::string> args);
  void SphereHandler(std::queue<std::string> args);
  void DirectionalHandler(std::queue<std::string> args);
  void PointHandler(std::queue<std::string> args);
  void VertexHandler(std::queue<std::string> args);
  void TriHandler(std::queue<std::string> args);
  void AmbientHandler(std::queue<std::string> args);
  void DiffuseHandler(std::queue<std::string> args);
  void SpecularHandler(std::queue<std::string> args);
  void EmissionHandler(std::queue<std::string> args);
  void ShininessHandler(std::queue<std::string> args);
  void PushTransformHandler(std::queue<std::string> args);
  void PopTransformHandler(std::queue<std::string> args);
  void ScaleHandler(std::queue<std::string> args);
  void TranslateHandler(std::queue<std::string> args);
  void RotateHandler(std::queue<std::string> args);
  void MaxDepthHandler(std::queue<std::string> args);
  void AttenuationHandler(std::queue<std::string> args);
};

#endif
