#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <string>
#include <queue>

class Scene;

class FileFormat {
  public:
  FileFormat(std::string filename);
  ~FileFormat(){};

  virtual void ReadIntoScene(Scene* scene) = 0;

  protected:
  std::string _filename;

  std::vector<std::string> ReadFile(const std::string& path);
  void Split(std::queue<std::string>& returnVector, const std::string& string, std::string& delimiter);
};

#endif
