#include "transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "scad.h"

namespace scad {

glm::vec3 Transform::Apply(const glm::vec3& p) const {
  glm::mat4 transform(1.0f);
  transform = glm::translate(transform, translation());
  transform = glm::rotate(transform, glm::radians((float)ry), glm::vec3(0, 1, 0));
  transform = glm::rotate(transform, glm::radians((float)rx), glm::vec3(1, 0, 0));
  transform = glm::rotate(transform, glm::radians((float)rz), glm::vec3(0, 0, 1));

  glm::vec4 transformed = transform * glm::vec4(p.x, p.y, p.z, 1);
  return glm::vec3(transformed.x, transformed.y, transformed.z);
}

Shape TransformList::Apply(const Shape& in) const {
  Shape shape = in;
  for (auto& transform : transforms_) {
    shape = transform.Apply(shape);
  }
  return shape;
}

glm::vec3 TransformList::Apply(const glm::vec3& in) const {
  glm::vec3 point = in;
  for (auto& transform : transforms_) {
    point = transform.Apply(point);
  }
  return point;
}

}  // namespace scad
