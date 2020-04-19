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

  //
  // Start connecting the keys and building the walls.
  //

  std::vector<Key*> all_keys = d.all_keys();

  std::vector<Key*> left_wall_keys = {&d.key_plus, &d.key_tab, &d.key_caps, &d.key_shift};
  std::vector<Key*> right_wall_keys = {&d.key_5, &d.key_t, &d.key_g, &d.key_b};

  /*
  // Adjust the switch widths.
  for (Key* key : d.row(0)) {
    key->extra_width_top = 8;
  }
  d.key_tilda.extra_width_bottom = 6;
  d.key_tilda.extra_width_right = 3;
  d.key_slash.extra_width_bottom = 8;
  d.key_left_arrow.extra_width_bottom = 8;
  d.key_slash.extra_width_left = 0;

  for (Key* key : left_wall_keys) {
    key->extra_width_left = 4;
  }
  for (Key* key : right_wall_keys) {
    key->extra_width_right = 4;
  }
  // This is shifted out a little to avoid hitting 4. Try to keep the right wall
  // straight
  d.key_5.extra_width_right = 3;
  for (Key* key : d.column(2)) {
    // s column
    key->extra_width_left = 4;
  }
  d.key_2.extra_width_left += 1.5;
  d.key_3.extra_width_left += 1;
  for (Key* key : d.column(1)) {
    // a column
    key->extra_width_left = 3;
  }
  d.key_tilda.extra_width_left = 4.5;
  d.key_shift.extra_width_bottom = 8;

  for (Key* key : d.thumb_keys()) {
    key->extra_z += 1;
  }
  d.key_5.extra_z += 1.5;

  */

  //
  // Thumb plate
  //

  std::vector<Shape> thumb_plate_shapes;
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
  for (Key* key : d.thumb_keys()) {
    thumb_plate_shapes.push_back(key->GetSwitch());
  }
  thumb_plate_shapes.push_back(Union(
      ConnectHorizontal(d.thumb_ctrl, d.thumb_alt),
      ConnectHorizontal(d.thumb_backspace, d.thumb_delete),
      ConnectVertical(d.thumb_ctrl, d.thumb_delete),
      Tri(d.thumb_backspace.GetTopLeft(), d.thumb_delete.GetTopLeft(), d.thumb_ctrl.GetTopLeft())));
  Shape thumb_plate = UnionAll(thumb_plate_shapes);

  //
  // Main plate
  //
  std::vector<Shape> main_plate_shapes;
  d.key_b.extra_width_bottom = 10;
  for (Key* key : d.grid_keys()) {
    main_plate_shapes.push_back(key->GetSwitch());
  }
  main_plate_shapes.push_back(ConnectMainKeys(d));
  Shape main_plate = UnionAll(main_plate_shapes);

  std::vector<Shape> shapes;
  shapes.push_back(thumb_plate);
  shapes.push_back(main_plate);

  // Connect main and thumb plates.
  shapes.push_back(Union(
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

  /*
  shapes.push_back(
      Tri(d.thumb_ctrl.GetTopLeft(), d.thumb_ctrl.GetBottomLeft(), d.key_b.GetBottomRight()));
                       */
  /*
  shapes.push_back(Tri(d.key_right_arrow.GetBottomRight(),
                       d.thumb_backspace.GetTopLeft(),
                       d.key_b.GetBottomRight()));
                       */
  /*
  shapes.push_back(Tri(d.key_right_arrow.GetBottomRight(),
                       d.key_right_arrow.GetTopRight(),
                       d.key_b.GetBottomLeft()));
                       */

  double wall_connector_offset = -1;
  Shape wall_connector = Cube(2, 2, 4).TranslateZ(-2);
  // The bottom left corner is a little messy. Add another point to try and
  // clean it up.
  TransformList extra_tilda_wall_point =
      d.key_shift.GetBottomRight(wall_connector_offset).TranslateFront(2, 0, 0);

  // Also messy where main meets thumb
  TransformList extra_thumb_top_wall_point =
      d.thumb_ctrl.GetTopLeft(wall_connector_offset).TranslateFront(-6, 0, -4);

  std::vector<TransformList> wall_points = {
      d.key_shift.GetBottomRight(wall_connector_offset),
      d.key_shift.GetBottomLeft(wall_connector_offset),
      d.key_shift.GetTopLeft(wall_connector_offset),

      d.key_caps.GetBottomLeft(wall_connector_offset),
      d.key_caps.GetTopLeft(wall_connector_offset),

      d.key_tab.GetBottomLeft(wall_connector_offset),
      d.key_tab.GetTopLeft(wall_connector_offset),

      d.key_plus.GetBottomLeft(wall_connector_offset),
      d.key_plus.GetTopLeft(wall_connector_offset),
      d.key_plus.GetTopRight(wall_connector_offset),

      d.key_1.GetTopLeft(wall_connector_offset),
      d.key_1.GetTopRight(wall_connector_offset),

      d.key_2.GetTopLeft(wall_connector_offset),
      d.key_2.GetTopRight(wall_connector_offset),

      d.key_3.GetTopLeft(wall_connector_offset),
      d.key_3.GetTopRight(wall_connector_offset),

      d.key_4.GetTopLeft(wall_connector_offset),
      d.key_4.GetTopRight(wall_connector_offset),

      d.key_5.GetTopLeft(wall_connector_offset),
      d.key_5.GetTopRight(wall_connector_offset),
      d.key_5.GetBottomRight(wall_connector_offset),

      d.key_t.GetTopRight(wall_connector_offset),
      d.key_t.GetBottomRight(wall_connector_offset),

      d.key_g.GetTopRight(wall_connector_offset),
      d.key_g.GetBottomRight(wall_connector_offset),

      d.key_b.GetTopRight(wall_connector_offset),

      extra_thumb_top_wall_point,

      d.thumb_ctrl.GetTopLeft(wall_connector_offset),
      d.thumb_ctrl.GetTopRight(wall_connector_offset),

      d.thumb_alt.GetTopLeft(wall_connector_offset),
      d.thumb_alt.GetTopRight(wall_connector_offset),
      d.thumb_alt.GetBottomRight(wall_connector_offset),

      d.thumb_home.GetTopRight(wall_connector_offset),
      d.thumb_home.GetBottomRight(wall_connector_offset),

      d.thumb_end.GetTopRight(wall_connector_offset),
      d.thumb_end.GetBottomRight(wall_connector_offset),
      d.thumb_end.GetBottomLeft(wall_connector_offset),

      d.thumb_delete.GetBottomRight(wall_connector_offset),
      d.thumb_delete.GetBottomLeft(wall_connector_offset),

      d.thumb_backspace.GetBottomRight(wall_connector_offset),
      d.thumb_backspace.GetBottomLeft(wall_connector_offset),

      d.key_left_arrow.GetBottomRight(wall_connector_offset),
      d.key_left_arrow.GetBottomLeft(wall_connector_offset),

      d.key_slash.GetBottomRight(wall_connector_offset),
      d.key_slash.GetBottomLeft(wall_connector_offset),

      d.key_tilda.GetBottomRight(wall_connector_offset),
      d.key_tilda.GetBottomLeft(wall_connector_offset),
      d.key_tilda.GetTopLeft(wall_connector_offset),
  };

  auto make_wall = [=](const std::vector<TransformList>& points) {
    auto get_wall_post = [=](TransformList t) {
      Shape s = t.Apply(wall_connector);
      return Hull(s, s.Projection().LinearExtrude(.01));
    };
    std::vector<Shape> wall_shapes;
    for (size_t i = 0; i < points.size() - 1; ++i) {
      wall_shapes.push_back(Hull(get_wall_post(points[i]), get_wall_post(points[i + 1])));
    }
    return UnionAll(wall_shapes);
  };

  // shapes.push_back(make_wall(wall_points));

  /*
  // Connect up thumb to main keys.
  shapes.push_back(Tri(extra_thumb_top_wall_point,
                       d.thumb_ctrl.GetTopLeft(wall_connector_offset),
                       d.thumb_delete.GetTopLeft(wall_connector_offset),
                       wall_connector));
  {
    TransformList b_point = d.key_b.GetBottomRight().TranslateFront(-2, 1, -4);
    TransformList a_point = d.key_right_arrow.GetTopRight(-1).TranslateFront(4, 2.5, 0);
    // TODO: These should be broken down into triangles.
    shapes.push_back(Hull(d.thumb_delete.GetTopLeft(wall_connector_offset).Apply(wall_connector),
                          b_point.Apply(wall_connector),
                          a_point.Apply(wall_connector),
                          d.thumb_backspace.GetTopLeft(wall_connector_offset).Apply(wall_connector),
                          d.key_right_arrow.GetTopRight(0).Apply(wall_connector)));
  }

  {
    TransformList extra_thumb_wall_point =
        d.key_right_arrow.GetBottomRight(wall_connector_offset).TranslateFront(-.5, 1, -3);
    // TODO: These should be broken down into triangles.
    shapes.push_back(
        Hull(d.thumb_backspace.GetBottomLeft(wall_connector_offset).Apply(wall_connector),
             extra_thumb_wall_point.Apply(wall_connector),
             d.key_right_arrow.GetBottomLeft(wall_connector_offset).Apply(wall_connector),
             d.key_left_arrow.GetBottomRight(wall_connector_offset).Apply(wall_connector)));
  }
  */

  if (kAddCaps) {
    for (Key* key : d.all_keys()) {
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
  // thumb_backspace key hole needs cutting
  // right arrow key hole needs cutting

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
