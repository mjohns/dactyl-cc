#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "key.h"
#include "key_data.h"
#include "scad.h"
#include "transform.h"

using namespace scad;

constexpr bool kWriteTestKeys = false;
// Add the caps into the stl for testing.
constexpr bool kAddCaps = false;

enum class Direction { UP, DOWN, LEFT, RIGHT };

Shape ConnectMainKeys(KeyData& d);

int main() {
  TransformList key_origin;
  key_origin.Translate(-20, -40, 3);

  // This is where all of the logic to position the keys is done. Everything below is cosmetic
  // trying to build the case.
  KeyData d(key_origin);

  if (kWriteTestKeys) {
    std::vector<Shape> test_shapes;
    std::vector<Key*> test_keys = {&d.key_3, &d.key_e, &d.key_4};
    for (Key* key : test_keys) {
      key->add_side_nub = false;
      key->extra_z = 4;
      test_shapes.push_back(key->GetSwitch());
      if (kAddCaps) {
        test_shapes.push_back(key->GetCap().Color("red"));
      }
    }
    UnionAll(test_shapes).WriteToFile("test_keys.scad");
    return 0;
  }

  // Set all of the widths here. This must be done before calling any of GetTopLeft etc.

  d.thumb_backspace.extra_width_bottom = 2;
  d.thumb_backspace.extra_width_left = 2;
  d.thumb_delete.extra_width_bottom = 2;
  d.thumb_end.extra_width_bottom = 2;
  d.thumb_ctrl.extra_width_top = 2;
  d.thumb_alt.extra_width_top = 2;
  d.thumb_alt.extra_width_right = 2;
  d.thumb_alt.extra_width_left = 2;
  d.thumb_home.extra_width_right = 2;
  d.thumb_home.extra_width_left = 2;
  d.thumb_home.extra_width_top = 2;
  d.thumb_end.extra_width_top = 2;
  d.thumb_end.extra_width_right = 2;
  d.thumb_end.extra_width_left = 2;

  // left wall
  for (Key* key : d.column(0)) {
    if (key) {
      key->extra_width_left = 4;
    }
  }

  d.key_5.extra_width_right = 2;
  d.key_t.extra_width_right = 3;
  d.key_g.extra_width_right = 3;

  for (Key* key : d.row(0)) {
    // top row
    if (key) {
      key->extra_width_top = 2;
    }
  }
  d.key_b.extra_width_bottom = 3;

  std::vector<Shape> shapes;

  //
  // Thumb plate
  //

  shapes.push_back(Union(ConnectHorizontal(d.thumb_ctrl, d.thumb_alt),
                         ConnectHorizontal(d.thumb_backspace, d.thumb_delete),
                         ConnectVertical(d.thumb_ctrl, d.thumb_delete),
                         Tri(d.thumb_end.GetBottomLeft(),
                             d.thumb_delete.GetBottomRight(),
                             d.thumb_backspace.GetBottomLeft())));

  shapes.push_back(ConnectMainKeys(d));

  shapes.push_back(TriFan(d.thumb_ctrl.GetTopLeft(),
                          {
                              d.key_b.GetBottomRight(),
                              d.key_b.GetTopRight(),
                              d.key_g.GetBottomRight(),
                          })

  );

  // These transforms with TranslateFront are moving the connectors down in the z direction to
  // reduce the vertical jumps.
  TransformList slash_bottom_right = d.key_slash.GetBottomRight().TranslateFront(0, 0, -1);

  shapes.push_back(TriFan(d.thumb_backspace.GetBottomLeft(),
                          {
                              d.thumb_backspace.GetBottomLeft(),
                              d.key_right_arrow.GetBottomRight(),
                              d.key_right_arrow.GetBottomLeft().TranslateFront(0, 0, -1),
                              d.key_left_arrow.GetBottomRight().TranslateFront(0, 0, -1),
                              d.key_left_arrow.GetBottomLeft(),
                              slash_bottom_right,
                          }));
  shapes.push_back(TriFan(d.key_tilde.GetBottomRight(),
                          {
                              d.key_slash.GetBottomLeft(),
                              slash_bottom_right,
                          }));
  shapes.push_back(TriFan(d.thumb_delete.GetTopLeft(),
                          {
                              d.thumb_ctrl.GetTopLeft(),
                              d.key_b.GetBottomRight(),
                              d.thumb_backspace.GetTopLeft(),
                          }));
  shapes.push_back(TriFan(d.key_b.GetBottomLeft(),
                          {
                              d.key_b.GetBottomRight(),
                              d.thumb_backspace.GetTopLeft(),
                              d.thumb_backspace.GetTopLeft(),
                              d.key_right_arrow.GetBottomRight(),
                              d.key_right_arrow.GetTopRight(),
                              d.key_v.GetBottomRight(),
                          }));

  // Bottom right corner.
  shapes.push_back(TriFan(d.key_shift.GetBottomRight(),
                          {
                              d.key_z.GetBottomLeft(),
                              d.key_tilde.GetTopLeft(),
                              d.key_tilde.GetBottomLeft(),
                              d.key_shift.GetBottomLeft(),
                          }));

  //
  // Make the wall
  //
  // TODO: Consider adding a width parameter to help with certain segments of the wall which are
  // thinner.
  {
    struct WallPoint {
      WallPoint(TransformList transforms, Direction out_direction, float distance = 4.0)
          : transforms(transforms), out_direction(out_direction), distance(distance) {
      }
      TransformList transforms;
      Direction out_direction;
      float distance;
    };

    Direction up = Direction::UP;
    Direction down = Direction::DOWN;
    Direction left = Direction::LEFT;
    Direction right = Direction::RIGHT;

    std::vector<WallPoint> wall_points = {
        // Start top left and go clockwise
        {d.key_plus.GetTopLeft(), up},
        {d.key_plus.GetTopRight(), up},

        {d.key_1.GetTopLeft(), up},
        {d.key_1.GetTopRight(), up},

        {d.key_2.GetTopLeft(), up},
        {d.key_2.GetTopRight(), up},

        {d.key_3.GetTopLeft(), up},
        {d.key_3.GetTopRight(), up},

        {d.key_4.GetTopLeft(), up},
        {d.key_4.GetTopRight(), up},

        {d.key_5.GetTopRight(), up},
        {d.key_5.GetTopRight(), right},
        {d.key_5.GetBottomRight(), right},

        {d.key_t.GetTopRight(), right},
        {d.key_t.GetBottomRight(), right},

        {d.key_g.GetTopRight(), right},
        {d.key_g.GetBottomRight(), right, 5},

        {d.thumb_ctrl.GetTopLeft(), up, 5},
        {d.thumb_ctrl.GetTopRight(), up},

        {d.thumb_alt.GetTopLeft(), up},
        {d.thumb_alt.GetTopRight(), up},
        {d.thumb_alt.GetTopRight(), right},
        {d.thumb_alt.GetBottomRight(), right},

        {d.thumb_home.GetTopRight(), right},
        {d.thumb_home.GetBottomRight(), right},

        {d.thumb_end.GetTopRight(), right},
        {d.thumb_end.GetBottomRight(), right},
        {d.thumb_end.GetBottomRight(), down},
        {d.thumb_end.GetBottomLeft(), down},

        {d.thumb_backspace.GetBottomLeft(), down},

        {slash_bottom_right, down},

        {d.key_tilde.GetBottomRight(), down},
        {d.key_tilde.GetBottomLeft(), down},

        {d.key_shift.GetBottomLeft(), down},
        {d.key_shift.GetBottomLeft(), left},
        {d.key_shift.GetTopLeft(), left},

        {d.key_caps.GetBottomLeft(), left},
        {d.key_caps.GetTopLeft(), left},

        {d.key_tab.GetBottomLeft(), left},
        {d.key_tab.GetTopLeft(), left},

        {d.key_plus.GetBottomLeft(), left},
        {d.key_plus.GetTopLeft(), left},
    };

    std::vector<std::vector<Shape>> wall_slices;
    for (WallPoint point : wall_points) {
      Shape s1 = point.transforms.Apply(GetPostConnector());

      TransformList t = point.transforms;
      glm::vec3 out_dir;
      switch (point.out_direction) {
        case Direction::UP:
          t.AppendFront(TransformList().Translate(0, point.distance, 0).RotateX(-20));
          break;
        case Direction::DOWN:
          t.AppendFront(TransformList().Translate(0, -1 * point.distance, 0).RotateX(20));
          break;
        case Direction::LEFT:
          t.AppendFront(TransformList().Translate(-1 * point.distance, 0, 0).RotateY(-20));
          break;
        case Direction::RIGHT:
          t.AppendFront(TransformList().Translate(point.distance, 0, 0).RotateY(20));
          break;
      }

      // Make sure the section extruded to the bottom is thick enough. With certain angles the
      // projection is very small if you just use the post connector from the transform. Compute
      // an explicit shape.
      const glm::vec3 post_offset(0, 0, -4);
      const glm::vec3 p = point.transforms.Apply(post_offset);
      const glm::vec3 p2 = t.Apply(post_offset);

      glm::vec3 out_v = p2 - p;
      out_v.z = 0;
      const glm::vec3 in_v = -1.f * glm::normalize(out_v);

      Shape s2 = Hull(Cube(.1).Translate(p2), Cube(.1).Translate(p2 + (2.5f * in_v)));

      std::vector<Shape> slice;
      slice.push_back(Hull(s1, s2));
      slice.push_back(Hull(s2, s2.Projection().LinearExtrude(.1).TranslateZ(.05)));

      wall_slices.push_back(slice);
    }

    for (size_t i = 0; i < wall_slices.size(); ++i) {
      auto& slice = wall_slices[i];
      auto& next_slice = wall_slices[(i + 1) % wall_slices.size()];
      for (size_t j = 0; j < slice.size(); ++j) {
        shapes.push_back(Hull(slice[j], next_slice[j]));
        // Uncomment for testing. Much faster and easier to visualize.
        // shapes.push_back(slice[j]);
      }
    }
  }

  for (Key* key : d.all_keys()) {
    shapes.push_back(key->GetSwitch());
    if (kAddCaps) {
      shapes.push_back(key->GetCap().Color("red"));
    }
  }

  // Add all the screw inserts.
  {
    double screw_height = 5;
    double screw_radius = 4.4 / 2.0;
    Shape screw_insert = Cylinder(screw_height, screw_radius + 1.65, 30)
                             .Subtract(Cylinder(screw_height + 2, screw_radius, 30))
                             .TranslateZ(screw_height / 2);

    glm::vec3 screw_left_bottom = d.key_shift.GetBottomLeft().Apply(kOrigin);
    screw_left_bottom.z = 0;
    screw_left_bottom.x += 2.8;

    glm::vec3 screw_left_top = d.key_plus.GetTopLeft().Apply(kOrigin);
    screw_left_top.z = 0;
    screw_left_top.x += 2.8;
    screw_left_top.y += -.5;

    glm::vec3 screw_right_top = d.key_5.GetTopRight().Apply(kOrigin);
    screw_right_top.z = 0;
    screw_right_top.x -= .8;
    screw_right_top.y += -.5;

    glm::vec3 screw_right_bottom = d.thumb_end.GetBottomLeft().Apply(kOrigin);
    screw_right_bottom.z = 0;
    screw_right_bottom.y += 3.5;
    screw_right_bottom.x += 1.5;

    glm::vec3 screw_right_mid = d.thumb_ctrl.GetTopLeft().Apply(kOrigin);
    screw_right_mid.z = 0;
    screw_right_mid.y += -.3;

    shapes.push_back(Union(screw_insert.Translate(screw_left_top),
                           screw_insert.Translate(screw_right_top),
                           screw_insert.Translate(screw_right_mid),
                           screw_insert.Translate(screw_right_bottom),
                           screw_insert.Translate(screw_left_bottom)));
  }

  std::vector<Shape> negative_shapes;
  // Cut off the parts sticking up into the thumb plate.
  negative_shapes.push_back(
      d.thumb_backspace.GetTopLeft().Apply(Cube(30, 50, 6).Translate(15 - 0, 25 - 5, 3)));
  negative_shapes.push_back(
      d.thumb_backspace.GetTopLeft().Apply(Cube(50, 50, 6).TranslateZ(3)).Color("red"));

  // Cut out holes for cords. Inserts can be printed to fit in.
  glm::vec3 connector_location = d.key_4.GetTopLeft().Apply(glm::vec3(9, 0, -4));
  connector_location.z = 0;
  negative_shapes.push_back(Cube(10, 10, 15).TranslateZ(5).Translate(connector_location));

  Shape result = UnionAll(shapes);
  // Subtracting is expensive to preview and is best to disable while testing.
  result = result.Subtract(UnionAll(negative_shapes));
  result.WriteToFile("left.scad");
  return 0;
}

Shape ConnectMainKeys(KeyData& d) {
  std::vector<Shape> shapes;
  for (int r = 0; r < d.num_rows(); ++r) {
    for (int c = 0; c < d.num_columns(); ++c) {
      Key* key = d.get_key(r, c);
      if (!key) {
        // No key at this location.
        continue;
      }
      Key* left = d.get_key(r, c - 1);
      Key* top_left = d.get_key(r - 1, c - 1);
      Key* top = d.get_key(r - 1, c);

      if (left) {
        shapes.push_back(ConnectHorizontal(*left, *key));
      }
      if (top) {
        shapes.push_back(ConnectVertical(*top, *key));
        if (left && top_left) {
          shapes.push_back(ConnectDiagonal(*top_left, *top, *key, *left));
        }
      }
    }
  }
  return UnionAll(shapes);
}
