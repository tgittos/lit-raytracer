#include "FileFormat.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>

FileFormat::FileFormat(std::string filename)
  :_filename(filename) {}

std::vector<std::string> FileFormat::ReadFile(const std::string& filename) {
  std::vector<std::string> lines;
  std::ifstream fileStream(filename.c_str());
  if (fileStream.is_open()){
    while (fileStream.good()) {
      std::string line;
      std::getline(fileStream, line);
      lines.push_back(line);
    }
    fileStream.close();
  }
  return lines;
}

void FileFormat::Split(std::queue<std::string>& returnVector, const std::string& theString, std::string& delimiter) {
  size_t start = 0, end = 0;
  while(end != std::string::npos) {
    end = theString.find(delimiter, start);
    if (start == end) {
      // double delimiter, advance by the delimiter size
      start += delimiter.size();
      continue;
    }
    returnVector.push(theString.substr(start, (end == std::string::npos) ? std::string::npos : end - start));
    start = ((end > (std::string::npos - delimiter.size())) ? std::string::npos : end + delimiter.size());
  }
}
