#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "scad.h"

namespace scad {

const glm::vec3 kOrigin(0, 0, 0);

// A rotation and translation. The rotations are applied first in z,x,y order and then the
// translation is added.
struct Transform {
 public:
  float x = 0;
  float y = 0;
  float z = 0;

  // Rotation in degrees
  float rx = 0;
  float ry = 0;
  float rz = 0;

  Transform() {
  }

  Transform(double px, double py, double pz) {
    x = px;
    y = py;
    z = pz;
  }

  Transform(const glm::vec3& t) {
    x = t.x;
    y = t.y;
    z = t.z;
  }

  glm::vec3 translation() const {
    return glm::vec3(x, y, z);
  }

  Shape Apply(const Shape& in) const {
    Shape shape = in;
    if (rz != 0) {
      shape = shape.RotateZ(rz);
    }
    if (rx != 0) {
      shape = shape.RotateX(rx);
    }
    if (ry != 0) {
      shape = shape.RotateY(ry);
    }
    if (x != 0 || y != 0 || z != 0) {
      shape = shape.Translate(x, y, z);
    }
    return shape;
  }

  glm::vec3 Apply(const glm::vec3& p) const;
};

// A list of transforms to apply to a shape or a point. The rotations are applied in order.
class TransformList {
 public:
  Shape Apply(const Shape& shape) const;
  glm::vec3 Apply(const glm::vec3& p) const;

  Transform& AddTransform(Transform t = {}) {
    transforms_.push_back(t);
    return transforms_.back();
  }

  Transform& AddTransformFront(Transform t = {}) {
    transforms_.insert(transforms_.begin(), t);
    return transforms_.front();
  }

  bool empty() const {
    return transforms_.empty();
  }

  Transform& mutable_front() {
    if (empty()) {
      return AddTransform();
    }
    return transforms_.front();
  }

  TransformList& RotateX(float deg) {
    Transform& t = AddTransform();
    t.rx = deg;
    return *this;
  }

  TransformList& RotateY(float deg) {
    Transform& t = AddTransform();
    t.ry = deg;
    return *this;
  }

  TransformList& RotateZ(float deg) {
    Transform& t = AddTransform();
    t.rz = deg;
    return *this;
  }

  TransformList& TranslateFront(float x, float y, float z) {
    AddTransformFront({x, y, z});
    return *this;
  }

  TransformList& Translate(float x, float y, float z) {
    AddTransform({x, y, z});
    return *this;
  }

  TransformList& Translate(const glm::vec3& v) {
    return Translate(v.x, v.y, v.z);
  }

  TransformList& TranslateX(float x) {
    return Translate(x, 0, 0);
  }

  TransformList& TranslateY(float y) {
    return Translate(0, y, 0);
  }

  TransformList& TranslateZ(float z) {
    return Translate(0, 0, z);
  }

  TransformList& Append(const TransformList& other) {
    transforms_.insert(transforms_.end(), other.transforms_.begin(), other.transforms_.end());
    return *this;
  }

 private:
  std::vector<Transform> transforms_;
};

}  // namespace scad
