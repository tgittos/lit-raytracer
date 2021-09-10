#include "Transform.hpp"

glm::mat3 Transform::rotate(const float degrees, const glm::vec3& axis)
{
  float radians = degrees * PI / 180.f;
  glm::mat3 identity = glm::mat3(1.0f);
  glm::mat3 dual = glm::mat3(
    0.f, -axis[2], axis[1],
    axis[2], 0.f, -axis[0],
    -axis[1], axis[0], 0.f
  );
  glm::mat3 tensor = glm::mat3(
    axis[0] * axis[0], axis[0] * axis[1], axis[0] * axis[2],
    axis[1] * axis[0], axis[1] * axis[1], axis[1] * axis[2],
    axis[2] * axis[0], axis[2] * axis[1], axis[2] * axis[2]
  );

  glm::mat3 rotation = (identity * std::cos(radians)) +
                  (dual * std::sin(radians)) +
                  tensor * (1 - std::cos(radians));

  return rotation;
}

glm::mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
    return glm::mat4(
      sx, 0.f, 0.f, 0.f,
      0.f, sy, 0.f, 0.f,
      0.f, 0.f, sz, 0.f,
      0.f, 0.f, 0.f, 1.f
    );
}

glm::mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
    return glm::mat4(
      1.f, 0.f, 0.f, tx,
      0.f, 1.f, 0.f, ty,
      0.f, 0.f, 1.f, tz,
      0.f, 0.f, 0.f, 1.f
    );
}

glm::vec3 Transform::upvector(const glm::vec3 &up, const glm::vec3 &zvec)
{
    glm::vec3 x = glm::cross(up,zvec);
    glm::vec3 y = glm::cross(zvec,x);
    glm::vec3 ret = glm::normalize(y);
    return ret;
}
