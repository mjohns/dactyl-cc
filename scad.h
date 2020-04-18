#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#if defined(__GNUC__) || defined(__GNUG__)
#define SCAD_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define SCAD_WARN_UNUSED_RESULT
#endif

namespace scad {

const int kTabSize = 2;

using ScadWriter = std::function<void(std::FILE* file, int indent_level)>;

template <typename T>
class Optional {
 public:
  Optional() {
  }

  Optional(T value) : value_(value), has_value_(true) {
  }

  T value() const {
    return value_;
  }

  bool has_value() const {
    return has_value_;
  }

 private:
  T value_;
  bool has_value_ = false;
};

struct LinearExtrudeParams {
  double height = 0;
  double twist = 0;
  double convexity = 10;
  int slices = 20;
  double scale = 1;
  bool center = true;
};

class Shape {
 public:
  Shape() {
  }
  explicit Shape(std::shared_ptr<ScadWriter> scad) : scad_(std::move(scad)) {
  }
  explicit Shape(ScadWriter scad) : scad_(std::make_shared<ScadWriter>(std::move(scad))) {
  }

  static Shape Composite(const std::function<void(std::FILE*)>& write_name,
                         const std::vector<Shape>& shapes);
  static Shape LiteralComposite(const std::string& name, const std::vector<Shape>& shapes);
  static Shape Primitive(const std::function<void(std::FILE*)>& scad_writer);
  static Shape LiteralPrimitive(const std::string& primitive);

  void WriteToFile(const std::string& file_name) const;
  void AppendScad(std::FILE* file, int indent_level) const;

  Shape SCAD_WARN_UNUSED_RESULT Translate(double x, double y, double z) const;
  Shape SCAD_WARN_UNUSED_RESULT TranslateX(double x) const;
  Shape SCAD_WARN_UNUSED_RESULT TranslateY(double y) const;
  Shape SCAD_WARN_UNUSED_RESULT TranslateZ(double z) const;
  template <typename Vec3>
  Shape SCAD_WARN_UNUSED_RESULT Translate(const Vec3& v) const {
    return Translate(v.x, v.y, v.z);
  }

  Shape SCAD_WARN_UNUSED_RESULT Mirror(double x, double y, double z) const;
  Shape SCAD_WARN_UNUSED_RESULT MirrorY() const {
    return Mirror(0, 1, 0);
  }
  Shape SCAD_WARN_UNUSED_RESULT MirrorX() const {
    return Mirror(1, 0, 0);
  }

  Shape SCAD_WARN_UNUSED_RESULT Rotate(double rx, double ry, double rz) const;
  Shape SCAD_WARN_UNUSED_RESULT Rotate(double degrees, double x, double y, double z) const;
  Shape SCAD_WARN_UNUSED_RESULT RotateX(double degrees) const;
  Shape SCAD_WARN_UNUSED_RESULT RotateY(double degrees) const;
  Shape SCAD_WARN_UNUSED_RESULT RotateZ(double degrees) const;

  Shape SCAD_WARN_UNUSED_RESULT LinearExtrude(const LinearExtrudeParams& params) const;
  Shape SCAD_WARN_UNUSED_RESULT LinearExtrude(double height) const;

  Shape SCAD_WARN_UNUSED_RESULT Color(double r, double g, double b, double a = 1.0) const;
  Shape SCAD_WARN_UNUSED_RESULT Color(const std::string& color, double a = 1) const;
  Shape SCAD_WARN_UNUSED_RESULT Alpha(double a) const;

  Shape SCAD_WARN_UNUSED_RESULT Subtract(const Shape& other) const;
  Shape operator-(const Shape& other) const;
  Shape& operator-=(const Shape& other);

  Shape SCAD_WARN_UNUSED_RESULT Add(const Shape& other) const;
  Shape operator+(const Shape& other) const;
  Shape& operator+=(const Shape& other);

  Shape SCAD_WARN_UNUSED_RESULT Scale(double x, double y, double z) const;
  Shape SCAD_WARN_UNUSED_RESULT Scale(double s) const;

  Shape SCAD_WARN_UNUSED_RESULT OffsetRadius(double r, bool chamfer = false) const;
  Shape SCAD_WARN_UNUSED_RESULT OffsetDelta(double delta, bool chamfer = false) const;

  Shape SCAD_WARN_UNUSED_RESULT Comment(const std::string& comment) const;

  Shape SCAD_WARN_UNUSED_RESULT Projection(bool cut = false) const;

 private:
  std::shared_ptr<const ScadWriter> scad_;
};

struct CubeParams {
  double x = 1;
  double y = 1;
  double z = 1;
  bool center = true;
};
Shape SCAD_WARN_UNUSED_RESULT Cube(const CubeParams& params);
Shape SCAD_WARN_UNUSED_RESULT Cube(double x, double y, double z, bool center = true);
Shape SCAD_WARN_UNUSED_RESULT Cube(double size, bool center = true);

struct SphereParams {
  double r = 1;
  Optional<double> fn;
  Optional<double> fa;
  Optional<double> fs;
};
Shape SCAD_WARN_UNUSED_RESULT Sphere(const SphereParams& params);
Shape SCAD_WARN_UNUSED_RESULT Sphere(double radius);
Shape SCAD_WARN_UNUSED_RESULT Sphere(double radius, double fn);

struct CircleParams {
  double r = 1;
  Optional<double> fn;
  Optional<double> fa;
  Optional<double> fs;
};
Shape SCAD_WARN_UNUSED_RESULT Circle(const CircleParams& params);
Shape SCAD_WARN_UNUSED_RESULT Circle(double radius);
Shape SCAD_WARN_UNUSED_RESULT Circle(double radius, double fn);

struct CylinderParams {
  double h = 1;
  double r1 = 1;
  double r2 = 1;
  Optional<double> fn;
  bool center = true;
};
Shape SCAD_WARN_UNUSED_RESULT Cylinder(const CylinderParams& params);
Shape SCAD_WARN_UNUSED_RESULT Cylinder(double height, double radius, Optional<double> fn = {});

struct SquareParams {
  double x = 1;
  double y = 1;
  bool center = true;
};
Shape SCAD_WARN_UNUSED_RESULT Square(const CubeParams& params);
Shape SCAD_WARN_UNUSED_RESULT Square(double x, double y, bool center = true);
Shape SCAD_WARN_UNUSED_RESULT Square(double size, bool center = true);

struct Point2d {
  double x = 0;
  double y = 0;
};
Shape SCAD_WARN_UNUSED_RESULT Polygon(const std::vector<Point2d>& points);

Shape SCAD_WARN_UNUSED_RESULT RegularPolygon(int n, double radius);

struct Point3d {
  double x = 0;
  double y = 0;
  double z = 0;
};
Shape SCAD_WARN_UNUSED_RESULT Polyhedron(const std::vector<Point3d>& points,
                                         const std::vector<std::vector<int>>& faces,
                                         int convexity = 1);

Shape SCAD_WARN_UNUSED_RESULT HullAll(const std::vector<Shape>& shapes);

template <typename... Shapes>
Shape SCAD_WARN_UNUSED_RESULT Hull(const Shape& shape, const Shapes&... more_shapes) {
  return HullAll({shape, more_shapes...});
}

Shape SCAD_WARN_UNUSED_RESULT UnionAll(const std::vector<Shape>& shapes);

template <typename... Shapes>
Shape SCAD_WARN_UNUSED_RESULT Union(const Shape& shape, const Shapes&... more_shapes) {
  return UnionAll({shape, more_shapes...});
}

Shape SCAD_WARN_UNUSED_RESULT DifferenceAll(const std::vector<Shape>& shapes);

template <typename... Shapes>
Shape SCAD_WARN_UNUSED_RESULT Difference(const Shape& shape, const Shapes&... more_shapes) {
  return DifferenceAll({shape, more_shapes...});
}

Shape SCAD_WARN_UNUSED_RESULT IntersectionAll(const std::vector<Shape>& shapes);

template <typename... Shapes>
Shape SCAD_WARN_UNUSED_RESULT Intersection(const Shape& shape, const Shapes&... more_shapes) {
  return IntersectionAll({shape, more_shapes...});
}

Shape SCAD_WARN_UNUSED_RESULT Import(const std::string& file_name, int convexity = -1);

Shape SCAD_WARN_UNUSED_RESULT Minkowski(const Shape& first, const Shape& second);

const char* BoolStr(bool b);
void WriteIndent(std::FILE* file, int indent_level);
void WriteComposite(std::FILE* file,
                    const std::function<void(std::FILE*)>& write_name,
                    const std::vector<Shape>& shapes,
                    int indent_level);

}  // namespace scad
