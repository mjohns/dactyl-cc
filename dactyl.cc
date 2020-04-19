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

Shape ConnectMainKeys(KeyData& d);

int main() {
  TransformList key_origin;
  key_origin.Translate(0, 0, 10);

  // This is where all of the logic to position the keys is done. Everything below is cosmetic
  // trying to build the case.
  KeyData d(key_origin);

  if (kWriteTestKeys) {
    std::vector<Shape> test_shapes;
    std::vector<Key*> test_keys = {&d.key_5, &d.key_4};
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

  // Set all of the widths here.

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

  d.key_left_arrow.extra_width_bottom = 5;
  d.key_slash.extra_width_bottom = 8;
  d.key_tilda.extra_width_bottom = 4;
  d.key_shift.extra_width_bottom = 6;

  //
  // Start connecting the keys and building the walls.
  //

  std::vector<Shape> shapes;

  // Adjust the switch widths.

  for (Key* key : d.row(0)) {
    // top row
    key->extra_width_top = 4;
  }
  d.key_1.extra_width_top += 3;

  for (Key* key : d.column(0)) {
    if (key != nullptr) {
      key->extra_width_left = 4;
    }
  }
  // This is shifted out a little to avoid hitting 4. Try to keep the right wall
  // straight
  d.key_5.extra_width_right = 3;
  d.key_t.extra_width_right = 4;
  d.key_g.extra_width_right = 4;

  // d.key_5.extra_z += 1.5;


  //
  // Thumb plate
  //

  shapes.push_back(Union(
      ConnectHorizontal(d.thumb_ctrl, d.thumb_alt),
      ConnectHorizontal(d.thumb_backspace, d.thumb_delete),
      ConnectVertical(d.thumb_ctrl, d.thumb_delete),
      Tri(d.thumb_end.GetBottomLeft(),
          d.thumb_delete.GetBottomRight(),
          d.thumb_backspace.GetBottomLeft()),
      Tri(d.thumb_backspace.GetTopLeft(), d.thumb_delete.GetTopLeft(), d.thumb_ctrl.GetTopLeft())));

  //
  // Main plate
  //
  d.key_b.extra_width_bottom = 10;
  shapes.push_back(ConnectMainKeys(d));

  // Connect main and thumb plates.
  shapes.push_back(Union(
      Tri(d.thumb_ctrl.GetTopLeft(), d.key_b.GetTopRight(), d.key_g.GetBottomRight()),
      Tri(d.thumb_ctrl.GetTopLeft(),
          d.key_b.GetTopRight(),
          d.key_b.GetTopRight().TranslateFront(0, -22, 0)),
      Tri(d.thumb_backspace.GetBottomLeft(),
          d.key_right_arrow.GetBottomLeft(),
          d.thumb_backspace.GetTopLeft().TranslateFront(0, -4.5, 0)),
      Tri(d.key_right_arrow.GetBottomRight(),
          d.key_right_arrow.GetTopRight(),
          d.key_b.GetBottomLeft()),
      Tri(d.key_right_arrow.GetTopRight(), d.key_v.GetBottomRight(), d.key_b.GetBottomLeft()),
      Tri(d.key_right_arrow.GetBottomRight(), d.key_b.GetBottomLeft(), d.key_b.GetBottomRight())));

  // Working on where the thumb meets the arrow keys and bottom.
  shapes.push_back(Union(Tri(d.thumb_backspace.GetBottomLeft(),
                             d.key_left_arrow.GetBottomLeft(),
                             d.key_left_arrow.GetBottomRight()),
                         Tri(d.thumb_backspace.GetBottomLeft(),
                             d.key_left_arrow.GetBottomLeft(),
                             d.key_slash.GetBottomRight()),
                         Tri(d.thumb_backspace.GetBottomLeft(),
                             d.key_right_arrow.GetBottomLeft(),
                             d.key_left_arrow.GetBottomRight())));

  // Bottom right corner.
  shapes.push_back(
      Tri(d.key_shift.GetBottomRight(), d.key_tilda.GetBottomLeft(), d.key_tilda.GetTopLeft()));
  shapes.push_back(
      Tri(d.key_shift.GetBottomRight(), d.key_z.GetBottomLeft(), d.key_tilda.GetTopLeft()));
  shapes.push_back(
      Tri(d.key_tilda.GetBottomLeft(), d.key_shift.GetBottomLeft(), d.key_shift.GetBottomRight()));

  //
  // Make the wall
  //
  {
    const int slice_point_count = 6;
    struct WallPoint {
      WallPoint(TransformList transforms, glm::vec3 out_direction, float distance = 6.0)
          : transforms(transforms), out_direction(out_direction), distance(distance) {
      }
      TransformList transforms;
      glm::vec3 out_direction;
      float distance;
    };

    glm::vec3 up(0, 1, 0);
    glm::vec3 down(0, -1, 0);
    glm::vec3 left(-1, 0, 0);
    glm::vec3 right(1, 0, 0);

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
        {d.key_4.GetTopRight(), glm::vec3(-1, 3, 0)},

        //{d.key_5.GetTopLeft(), glm::vec3(3, 3, 0), 10},
        {d.key_5.GetTopRight(), up},
        {d.key_5.GetTopRight(), right},
        {d.key_5.GetBottomRight(), right},

        {d.key_t.GetTopRight(), right},
        {d.key_t.GetBottomRight(), right},

        {d.key_g.GetTopRight(), right},
        {d.key_g.GetBottomRight(), glm::vec3(2, 1, 0)},

        {d.thumb_ctrl.GetTopLeft(), glm::vec3(4, 5, 0)},
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

        {d.key_slash.GetBottomRight(), down},
        {d.key_slash.GetBottomLeft(), glm::vec3(1, -2, 0)},

        {d.key_tilda.GetBottomRight(), down},
        {d.key_tilda.GetBottomLeft(), down},
        {d.key_tilda.GetBottomLeft(), glm::vec3(-1, -1, 0)},

        {d.key_shift.GetBottomLeft(), down},
        {d.key_shift.GetBottomLeft(), glm::vec3(-1, -1, 0)},
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
      point.out_direction.z = 0;
      const glm::vec3 out = glm::normalize(point.out_direction);

      glm::vec3 p = point.transforms.Apply(kOrigin);

      Shape last_shape = Hull(point.transforms.Apply(GetPostConnector()), Cube(.1).Translate(p - (1.5f * out)).TranslateZ(-2));
      std::vector<Shape> slice;
      const double z_step = p.z / slice_point_count;
      float step = point.distance;
      for (int i = 0; i < slice_point_count; ++i) {
        p = p + (step * out);
        p.z -= z_step;
        step = step / 2;

        Shape s = Hull(Cube(.1).Translate(p), Cube(.1).Translate(p - (out * 3.f)));
        slice.push_back(Hull(last_shape, s));
        last_shape = s;
      }

      wall_slices.push_back(slice);
    }

    for (size_t i = 0; i < wall_slices.size(); ++i) {
      auto& slice = wall_slices[i];
      auto& next_slice = wall_slices[(i + 1) % wall_slices.size()];
      for (size_t j = 0; j < slice_point_count; ++j) {
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

  std::vector<Shape> negative_shapes;
  // Cut off the parts sticking up into the thumb plate.
  negative_shapes.push_back(
      d.thumb_backspace.GetTopLeft().Apply(Cube(30, 30, 6).Translate(15 - 0, 15 - 5, 3)));
  negative_shapes.push_back(
      d.thumb_backspace.GetTopLeft()
          .Apply(Cube(10, 20, 2).Translate(5, 0, 1).RotateY(-30).TranslateY(-4))
          .Color("red"));
  negative_shapes.push_back(d.thumb_backspace.GetInverseSwitch());
  negative_shapes.push_back(d.key_right_arrow.GetInverseSwitch());

  UnionAll(shapes).Subtract(UnionAll(negative_shapes).Color("red")).WriteToFile("left.scad");
  return 0;
}

Shape ConnectMainKeys(KeyData& d) {
  std::vector<Shape> shapes;
  for (int r = 0; r < d.num_rows(); ++r) {
    for (int c = 0; c < d.num_columns(); ++c) {
      Key* key = d.get_key(r, c);
      if (key == nullptr) {
        // No key at this location.
        continue;
      }
      Key* left = d.get_key(r, c - 1);
      Key* top_left = d.get_key(r - 1, c - 1);
      Key* top = d.get_key(r - 1, c);

      if (left != nullptr) {
        shapes.push_back(ConnectHorizontal(*left, *key));
      }
      if (top != nullptr) {
        shapes.push_back(ConnectVertical(*top, *key));
        if (left != nullptr && top_left != nullptr) {
          shapes.push_back(ConnectDiagonal(*top_left, *top, *key, *left));
        }
      }
    }
  }
  return UnionAll(shapes);
}
