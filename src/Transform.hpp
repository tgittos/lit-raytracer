#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
  public:
  const static float PI = 3.14159265;

  Transform(){};
  ~Transform(){};

  static glm::mat3 rotate(const float degrees, const glm::vec3& axis);

  static glm::mat4 scale(const float &sx, const float &sy, const float &sz);

  static glm::mat4 translate(const float &tx, const float &ty, const float &tz);

  static glm::vec3 upvector(const glm::vec3 &up, const glm::vec3 &zvec);
};

#endif
