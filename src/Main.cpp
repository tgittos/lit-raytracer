#include <iostream>
#include "CS1841X.hpp"
#include "Scene.hpp"

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Need to specify path to scene" << std::endl;
    return 0;
  }
  std::string scenePath = std::string(argv[1]);
  Scene scene;
  CS1841X file(scenePath);
  file.ReadIntoScene(&scene);
  scene.RenderToImage();
  return 0;
}
