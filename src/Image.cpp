#include <iostream>

#include "Image.hpp"

Image::Image(int width, int height)
  :_width(width), _height(height) {
  long numPixels = width * height;
  _color = new byte[3 * numPixels];
  for (long i = 0; i < 3 * numPixels; i++) {
    _color[i] = 0;
  }
}

Image::Image(const Image& other) {
    _color = new byte[_width * _height];
    memcpy(_color, other._color, sizeof(other._color));
}

Image& Image::operator=(const Image& other) {
    _color = new byte[_width * _height];
    memcpy(_color, other._color, sizeof(other._color));
  return *this;
}

Image::~Image() {
  delete _color;
}

void Image::SaveFile(const std::string filename) {
  FIBITMAP* img = FreeImage_ConvertFromRawBits(_color, _width, _height, _width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
  FreeImage_Save(FIF_PNG, img, filename.c_str(), 0);
  FreeImage_DeInitialise();
}

void Image::SetPixelColor(int x, int y, glm::vec3 color) {
  long offset = (x * 3) + (y * _width * 3);
  glm::vec3 result = color * glm::vec3(255, 255, 255);
  _color[offset] = result[2];
  _color[offset+1] = result[1];
  _color[offset+2] = result[0];
}
