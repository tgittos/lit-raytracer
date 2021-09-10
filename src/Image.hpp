#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FreeImage.h"

typedef unsigned char byte;

class Image {
  public:
  Image(){};
  Image(int width, int height);
  Image(const Image& other); //copy constructor
  Image& operator=(const Image& other); //copy assignment operator
  ~Image();

  void SetWidth(int value) { _width = value; }
  void SetHeight(int value) { _height = value; }

  void SetPixelColor(int x, int y, glm::vec3 color);
  void SaveFile(const std::string filename);

  private:
  byte *_color;
  int _width;
  int _height;
};

#endif
