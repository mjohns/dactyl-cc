#include "scad.h"

// Windows!
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <math.h>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

namespace scad {

const char* BoolStr(bool b) {
  return b ? "true" : "false";
}

void WriteIndent(std::FILE* file, int indent_level) {
  for (int i = 0; i < indent_level * kTabSize; ++i) {
    fputc(' ', file);
  }
}

void WriteComposite(std::FILE* file,
                    const std::function<void(std::FILE*)>& write_name,
                    const std::vector<Shape>& shapes,
                    int indent_level) {
  WriteIndent(file, indent_level);
  write_name(file);
  fprintf(file, " {\n");
  for (const Shape& s : shapes) {
    s.AppendScad(file, indent_level + 1);
  }
  WriteIndent(file, indent_level);
  fprintf(file, "}\n");
}

Shape Shape::Composite(const std::function<void(std::FILE*)>& write_name,
                       const std::vector<Shape>& shapes) {
  return Shape([=](std::FILE* file, int indent_level) {
    WriteComposite(file, write_name, shapes, indent_level);
  });
}

Shape Shape::LiteralComposite(const std::string& name, const std::vector<Shape>& shapes) {
  return Shape([=](std::FILE* file, int indent_level) {
    WriteComposite(
        file, [=](std::FILE*) { fprintf(file, "%s", name.c_str()); }, shapes, indent_level);
  });
}

Shape Shape::Primitive(const std::function<void(std::FILE*)>& scad_writer) {
  return Shape([=](std::FILE* file, int indent_level) {
    WriteIndent(file, indent_level);
    scad_writer(file);
    fprintf(file, "\n");
  });
}

Shape Shape::LiteralPrimitive(const std::string& primitive) {
  return Primitive([=](std::FILE* file) { fprintf(file, "%s", primitive.c_str()); });
}

Shape Cube(const CubeParams& params) {
  return Shape::Primitive([=](std::FILE* file) {
    fprintf(file,
            "cube (size = [ %.3f, %.3f, %.3f], center = %s);",
            params.x,
            params.y,
            params.z,
            BoolStr(params.center));
  });
}

Shape Cube(double x, double y, double z, bool center) {
  CubeParams params;
  params.x = x;
  params.y = y;
  params.z = z;
  params.center = center;
  return Cube(params);
}

Shape Cube(double size, bool center) {
  return Cube(size, size, size, center);
}

Shape Square(const SquareParams& params) {
  return Shape::Primitive([=](std::FILE* file) {
    fprintf(file,
            "square (size = [%.3f, %.3f], center = %s);",
            params.x,
            params.y,
            BoolStr(params.center));
  });
}

Shape Square(double x, double y, bool center) {
  SquareParams params;
  params.x = x;
  params.y = y;
  params.center = center;
  return Square(params);
}

Shape Square(double size, bool center) {
  return Square(size, size, center);
}

Shape Sphere(const SphereParams& params) {
  return Shape::Primitive([=](std::FILE* file) {
    fprintf(file, "sphere (r = %.3f", params.r);
    if (params.fs.has_value()) {
      fprintf(file, ", $fs = %.3f", params.fs.value());
    }
    if (params.fn.has_value()) {
      fprintf(file, ", $fn = %.3f", params.fn.value());
    }
    if (params.fa.has_value()) {
      fprintf(file, ", $fa = %.3f", params.fa.value());
    }
    fprintf(file, ");");
  });
}

Shape Sphere(double radius) {
  SphereParams params;
  params.r = radius;
  return Sphere(params);
}

Shape Sphere(double radius, double fn) {
  SphereParams params;
  params.r = radius;
  params.fn = fn;
  return Sphere(params);
}

Shape Circle(const CircleParams& params) {
  return Shape::Primitive([=](std::FILE* file) {
    fprintf(file, "circle (r = %.3f", params.r);
    if (params.fs.has_value()) {
      fprintf(file, ", $fs = %.3f", params.fs.value());
    }
    if (params.fn.has_value()) {
      fprintf(file, ", $fn = %.3f", params.fn.value());
    }
    if (params.fa.has_value()) {
      fprintf(file, ", $fa = %.3f", params.fa.value());
    }
    fprintf(file, ");");
  });
}

Shape Circle(double radius) {
  CircleParams params;
  params.r = radius;
  return Circle(params);
}

Shape Circle(double radius, double fn) {
  CircleParams params;
  params.r = radius;
  params.fn = fn;
  return Circle(params);
}

Shape Cylinder(const CylinderParams& params) {
  return Shape::Primitive([=](std::FILE* file) {
    fprintf(file,
            "cylinder(h = %.3f, r1 = %.3f, r2 = %.3f, center = %s",
            params.h,
            params.r1,
            params.r2,
            BoolStr(params.center));
    if (params.fn.has_value()) {
      fprintf(file, ", $fn = %.3f", params.fn.value());
    }
    fprintf(file, ");");
  });
}

Shape Cylinder(double height, double radius, Optional<double> fn) {
  CylinderParams params;
  params.h = height;
  params.r1 = radius;
  params.r2 = radius;
  params.fn = fn;
  return Cylinder(params);
}

Shape Polygon(const std::vector<Point2d>& points) {
  return Shape::Primitive([=](std::FILE* file) {
    fprintf(file, "polygon (points = [");
    for (size_t i = 0; i < points.size(); ++i) {
      const Point2d& p = points[i];
      if (i != 0) {
        fputc(',', file);
      }
      fprintf(file, "[%.3f, %.3f]", p.x, p.y);
    }
    fprintf(file, "]);");
  });
}

Shape RegularPolygon(int n, double r) {
  std::vector<Point2d> points;
  for (int i = 0; i < n; ++i) {
    double step = (2.0 * M_PI) / n;
    points.push_back({r * sin(step * i), r * cos(step * i)});
  }
  return Polygon(points);
}

Shape Polyhedron(const std::vector<Point3d>& points,
                 const std::vector<std::vector<int>>& faces,
                 int convexity) {
  return Shape::Primitive([=](std::FILE* file) {
    fprintf(file, "polyhedron (points = [");
    for (size_t i = 0; i < points.size(); ++i) {
      const Point3d& p = points[i];
      if (i > 0) {
        fputc(',', file);
      }
      fprintf(file, "[%.3f, %.3f, %.3f]", p.x, p.y, p.z);
    }
    fprintf(file, "], faces = [");
    for (size_t i = 0; i < faces.size(); ++i) {
      if (i > 0) {
        fputc(',', file);
      }
      const auto& face = faces[i];
      fprintf(file, "[");
      for (size_t f = 0; f < face.size(); ++f) {
        if (f != 0) {
          fputc(',', file);
        }
        fprintf(file, "%d", face[f]);
      }
      fprintf(file, "]");
    }
    fprintf(file, "], convexity = %d);", convexity);
  });
}

Shape HullAll(const std::vector<Shape>& shapes) {
  return Shape::LiteralComposite("hull ()", shapes);
}

Shape UnionAll(const std::vector<Shape>& shapes) {
  return Shape::LiteralComposite("union ()", shapes);
}

Shape DifferenceAll(const std::vector<Shape>& shapes) {
  return Shape::LiteralComposite("difference ()", shapes);
}

Shape IntersectionAll(const std::vector<Shape>& shapes) {
  return Shape::LiteralComposite("intersection ()", shapes);
}

Shape Shape::Translate(double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "translate ([%.3f, %.3f, %.3f])", x, y, z);
  };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::TranslateX(double x) const {
  return Translate(x, 0, 0);
}

Shape Shape::TranslateY(double y) const {
  return Translate(0, y, 0);
}

Shape Shape::TranslateZ(double z) const {
  return Translate(0, 0, z);
}

Shape Shape::Mirror(double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) { fprintf(file, "mirror ([%.3f, %.3f, %.3f])", x, y, z); };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::Rotate(double rx, double ry, double rz) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "rotate ([%.3f, %.3f, %.3f])", rx, ry, rz);
  };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::Rotate(double degrees, double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "rotate (a = %.3f, v = [%.3f, %.3f, %.3f])", degrees, x, y, z);
  };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::RotateX(double degrees) const {
  return Rotate(degrees, 1, 0, 0);
}

Shape Shape::RotateY(double degrees) const {
  return Rotate(degrees, 0, 1, 0);
}

Shape Shape::RotateZ(double degrees) const {
  return Rotate(degrees, 0, 0, 1);
}

Shape Shape::LinearExtrude(const LinearExtrudeParams& params) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file,
            "linear_extrude (height = %.3f, center = %s, convexity = %.3f, "
            "twist = %.3f, slices = %d, scale = %.3f)",
            params.height,
            BoolStr(params.center),
            params.convexity,
            params.twist,
            params.slices,
            params.scale);
  };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::LinearExtrude(double height) const {
  LinearExtrudeParams params;
  params.height = height;
  return LinearExtrude(params);
}

Shape Shape::Color(double r, double g, double b, double a) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "color (c = [%.3f, %.3f, %.3f, %.3f])", r, g, b, a);
  };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::Color(const std::string& color, double a) const {
  auto write_name = [=](std::FILE* file) { fprintf(file, "color (\"%s\", %f)", color.c_str(), a); };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::Alpha(double a) const {
  auto write_name = [=](std::FILE* file) { fprintf(file, "color (alpha = %.3f)", a); };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::Scale(double x, double y, double z) const {
  auto write_name = [=](std::FILE* file) { fprintf(file, "scale ([%.3f, %.3f, %.3f])", x, y, z); };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::Scale(double s) const {
  return Scale(s, s, s);
}

Shape Shape::OffsetRadius(double r, bool chamfer) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "offset (r = %.3f, chamfer = %s)", r, BoolStr(chamfer));
  };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::OffsetDelta(double delta, bool chamfer) const {
  auto write_name = [=](std::FILE* file) {
    fprintf(file, "offset (delta = %.3f, chamfer = %s)", delta, BoolStr(chamfer));
  };
  return Shape::Composite(write_name, {*this});
}

Shape Shape::Subtract(const Shape& other) const {
  return Difference(*this, other);
}

Shape Shape::operator-(const Shape& other) const {
  return Subtract(other);
}

Shape& Shape::operator-=(const Shape& other) {
  *this = Subtract(other);
  return *this;
}

Shape Shape::Add(const Shape& other) const {
  return Union(*this, other);
}

Shape Shape::operator+(const Shape& other) const {
  return Add(other);
}

Shape& Shape::operator+=(const Shape& other) {
  *this = Add(other);
  return *this;
}

Shape Shape::Comment(const std::string& comment) const {
  Shape shape_copy = *this;
  return Shape([=](std::FILE* file, int indent_level) {
    WriteIndent(file, indent_level);
    fprintf(file, "/* %s */\n", comment.c_str());
    shape_copy.AppendScad(file, indent_level);
  });
}

Shape Shape::Projection(bool cut) const {
  auto write_name = [=](std::FILE* file) { fprintf(file, "projection (cut = %s)", BoolStr(cut)); };
  return Shape::Composite(write_name, {*this});
}

void Shape::AppendScad(std::FILE* file, int indent_level) const {
  if (!scad_) {
    return;
  }
  (*scad_)(file, indent_level);
}

void Shape::WriteToFile(const std::string& file_name) const {
  std::FILE* file = nullptr;
  bool opened = false;
#ifdef _WIN32
  opened = fopen_s(&file, file_name.c_str(), "w") == 0;
#else
  file = std::fopen(file_name.c_str(), "w");
  opened = file != nullptr;
#endif

  if (!opened || file == nullptr) {
    fprintf(stderr, "Could not open file %s\n", file_name.c_str());
    return;
  }
  AppendScad(file, 0);
  std::fclose(file);
}

Shape Import(const std::string& file_name, int convexity) {
  return Shape::Primitive([=](std::FILE* file) {
    if (convexity > 0) {
      fprintf(file, "import (file = \"%s\", convexity = %d);", file_name.c_str(), convexity);
    } else {
      fprintf(file, "import (file = \"%s\");", file_name.c_str());
    }
  });
}

Shape Minkowski(const Shape& first, const Shape& second) {
  return Shape::LiteralComposite("minkowski ()", {first, second});
}

}  // namespace scad
