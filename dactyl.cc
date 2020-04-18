#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "key.h"
#include "scad.h"
#include "transform.h"

using namespace scad;

constexpr double kDefaultKeySpacing = 19;
// The direct distance between switch tops in the bowl.
constexpr double kBowlKeySpacing = 18;

constexpr int kNumColumns = 6;
constexpr int kNumRows = 5;

constexpr double kDColumnRadius = 55;
constexpr double kAColumnRadius = 70;
constexpr double kSColumnRadius = 65;
constexpr double kGColumnRadius = 65;
constexpr double kFColumnRadius = 70;
constexpr double kCapsColumnRadius = 60;

// Rotates a key about the x axis until it has traveled the direct distance (not
// on the arc).
Key GetRotatedKey(double radius, bool up);

Shape ConnectMainKeys(const std::vector<std::vector<Key*>>& key_grid);

std::vector<Key*> GetColumn(const std::vector<std::vector<Key*>>& key_grid,
                            int c) {
  std::vector<Key*> result;
  for (int r = 0; r < kNumRows; ++r) {
    Key* key = key_grid[r][c];
    result.push_back(key);
  }
  return result;
}

int main() {
  // This is the parent of all keys. If you want to tilt the entire keyboard
  // changes this.
  Key key_origin;
  key_origin.Configure([&](Key& k) { k.SetPosition(0, 0, 10); });

  //
  // Thumb keys
  //

  Key thumb;
  thumb.Configure([&](Key& k) {
    k.name = "thumb";
    k.SetParent(key_origin);
    k.SetPosition(60, -9.18, 42.83);
    k.t().rz = -21;
    k.t().rx = 12;
    k.t().ry = -4.5;
  });

  // Second thumb key.
  Key thumb_delete;
  thumb_delete.Configure([&](Key& k) {
    k.name = "thumb_delete";
    k.SetParent(thumb);
    k.SetPosition(kDefaultKeySpacing, 0, 0);
  });

  // Bottom side key.
  Key thumb_end;
  thumb_end.Configure([&](Key& k) {
    k.name = "thumb_end";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, -9, 0);
  });

  // Middle side key.
  Key thumb_home;
  thumb_home.Configure([&](Key& k) {
    k.name = "thumb_home";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, 10, 0);
  });

  // Top side key;
  Key thumb_alt;
  thumb_alt.Configure([&](Key& k) {
    k.name = "thumb_alt";
    k.SetParent(thumb_delete);
    k.SetPosition(kDefaultKeySpacing, 10 + kDefaultKeySpacing, 0);
  });

  // Top left key.
  Key thumb_ctrl;
  thumb_ctrl.Configure([&](Key& k) {
    k.name = "thumb_ctrl";
    k.SetParent(thumb_delete);
    k.SetPosition(0, 10 + kDefaultKeySpacing, 0);
  });

  std::vector<Key*> thumb_keys = {&thumb,      &thumb_delete, &thumb_end,
                                  &thumb_home, &thumb_alt,    &thumb_ctrl};

  //
  // Main bowl keys
  //

  // All keys in the dish are relative to d and then based off of their
  // associated key in the home row.
  Key key_d;
  key_d.Configure([&](Key& k) {
    k.name = "d";
    k.SetParent(key_origin);
    k.SetPosition(26.40, 50.32, 17.87);
    k.t().ry = -15;
  });

  Key key_f;
  key_f.Configure([&](Key& k) {
    k.name = "f";

    // Absolute:
    // k.SetPosition(44.3, 49.37, 28.1);
    // k.t().ry = -20;

    k.SetParent(key_d);
    k.SetPosition(19.938, -0.950, 5.249);
    k.t().ry = -5;
  });

  Key key_g;
  key_g.Configure([&](Key& k) {
    k.name = "g";

    // Absolute:
    // k.SetPosition(60.16, 48.06, 37.39);
    // k.t().ry = -30;

    k.SetParent(key_f);
    k.SetPosition(18.081, -1.310, 3.305);
    k.SetPosition(18.481, -1.310, 3.305);
    k.t().ry = -10;
  });

  Key key_s;
  key_s.Configure([&](Key& k) {
    k.name = "s";

    // Absolute:
    // k.SetPosition(6.09, 50.23, 18.05);
    // k.t().ry = -10;

    k.SetParent(key_d);
    k.SetPosition(-19.571, -0.090, 5.430);
    k.t().ry = 5;
  });

  Key key_a;
  key_a.Configure([&](Key& k) {
    k.name = "a";

    // Absolute:
    // k.SetPosition(-15.41, 44.06, 19.7);
    // k.t().ry = -10;

    k.SetParent(key_s);
    k.SetPosition(-20.887, -6.170, 5.358);
  });

  Key key_caps;
  key_caps.Configure([&](Key& k) {
    k.name = "caps";

    // Absolute:
    // k.SetPosition(-37.7, 48.06, 15.98);
    // k.t().ry = -5;

    k.SetParent(key_a);
    k.SetPosition(-22.597, 4.000, 0.207);
    k.t().ry = 5;
  });

  // D Column
  Key key_e = GetRotatedKey(kDColumnRadius, true);
  key_e.Configure([&](Key& k) {
    k.name = "e";
    k.SetParent(key_d);
  });

  // This key is rotated more than the others in the column.
  Key key_3 = GetRotatedKey(50, true);
  key_3.Configure([&](Key& k) {
    k.name = "3";
    k.SetParent(key_e);
  });

  Key key_c = GetRotatedKey(kDColumnRadius, false);
  key_c.Configure([&](Key& k) {
    k.name = "c";
    k.SetParent(key_d);
  });

  Key key_left_arrow = GetRotatedKey(kDColumnRadius, false);
  key_left_arrow.Configure([&](Key& k) {
    k.name = "left_arrow";
    k.SetParent(key_c);
  });

  // S column
  Key key_w = GetRotatedKey(kSColumnRadius, true);
  key_w.Configure([&](Key& k) {
    k.name = "w";
    k.SetParent(key_s);
  });

  Key key_2 = GetRotatedKey(kSColumnRadius, true);
  key_2.Configure([&](Key& k) {
    k.name = "2";
    k.SetParent(key_w);
  });

  Key key_x = GetRotatedKey(kSColumnRadius, false);
  key_x.Configure([&](Key& k) {
    k.name = "x";
    k.SetParent(key_s);
  });

  Key key_slash = GetRotatedKey(kSColumnRadius, false);
  key_slash.Configure([&](Key& k) {
    k.name = "slash";
    k.SetParent(key_x);
  });

  // F column
  Key key_r = GetRotatedKey(kFColumnRadius, true);
  key_r.Configure([&](Key& k) {
    k.name = "r";
    k.SetParent(key_f);
  });

  Key key_4 = GetRotatedKey(kFColumnRadius, true);
  key_4.Configure([&](Key& k) {
    k.name = "4";
    k.SetParent(key_r);
  });

  Key key_v = GetRotatedKey(kFColumnRadius, false);
  key_v.Configure([&](Key& k) {
    k.name = "v";
    k.SetParent(key_f);
  });

  Key key_right_arrow = GetRotatedKey(kFColumnRadius, false);
  key_right_arrow.Configure([&](Key& k) {
    k.name = "right_arrow";
    k.SetParent(key_v);
  });

  Key key_t = GetRotatedKey(kGColumnRadius, true);
  key_t.Configure([&](Key& k) {
    k.name = "t";
    k.SetParent(key_g);
  });

  Key key_5 = GetRotatedKey(kGColumnRadius, true);
  key_5.Configure([&](Key& k) {
    k.name = "5";
    k.SetParent(key_t);
    // 5 hits 4 a little.
    k.t().x = 1.2;
  });

  Key key_b = GetRotatedKey(kGColumnRadius, false);
  key_b.Configure([&](Key& k) {
    k.name = "b";
    k.SetParent(key_g);
  });

  // A column
  Key key_q = GetRotatedKey(kAColumnRadius, true);
  key_q.Configure([&](Key& k) {
    k.name = "q";
    k.SetParent(key_a);
  });

  Key key_1 = GetRotatedKey(kAColumnRadius, true);
  key_1.Configure([&](Key& k) {
    k.name = "1";
    k.SetParent(key_q);
  });

  Key key_z = GetRotatedKey(kAColumnRadius, false);
  key_z.Configure([&](Key& k) {
    k.name = "z";
    k.SetParent(key_a);
  });

  Key key_tilda = GetRotatedKey(kAColumnRadius, false);
  key_tilda.Configure([&](Key& k) {
    k.name = "tilda";
    k.SetParent(key_z);
  });

  // Caps column
  Key key_tab = GetRotatedKey(kCapsColumnRadius, true);
  key_tab.Configure([&](Key& k) {
    k.name = "tab";
    k.SetParent(key_caps);
  });

  Key key_plus = GetRotatedKey(kCapsColumnRadius, true);
  key_plus.Configure([&](Key& k) {
    k.name = "plus";
    k.SetParent(key_tab);
  });

  Key key_shift = GetRotatedKey(kCapsColumnRadius, false);
  key_shift.Configure([&](Key& k) {
    k.name = "shift";
    k.SetParent(key_caps);
  });

  // 0 is the caps lock column, 0 is the number row.
  // clang-format off
  std::vector<std::vector<Key*>> key_grid = {
    { &key_plus,   &key_1,      &key_2,      &key_3,           &key_4,            &key_5},
    { &key_tab,    &key_q,      &key_w,      &key_e,           &key_r,            &key_t},
    { &key_caps,   &key_a,      &key_s,      &key_d,           &key_f,            &key_g},
    { &key_shift,  &key_z,      &key_x,      &key_c,           &key_v,            &key_b},
    { nullptr,     &key_tilda,  &key_slash,  &key_left_arrow,  &key_right_arrow,  nullptr},
  };
  // clang-format on

  /*
  std::vector<Shape> test_shapes;
  std::vector<Key*> test_keys = {&key_5, &key_4};
  for (Key* key : test_keys) {
    key->add_side_nub = false;
    key->extra_z = 4;
    test_shapes.push_back(key->GetSwitch());
  }
  UnionAll(test_shapes).WriteToFile("test.scad");
  */

  //
  // Start connecting the keys and building the walls.
  //

  std::vector<Key*> all_keys = thumb_keys;
  for (auto keys : key_grid) {
    for (Key* key : keys) {
      if (key != nullptr) {
        all_keys.push_back(key);
      }
    }
  }

  std::vector<Key*> left_wall_keys = {&key_plus, &key_tab, &key_caps,
                                      &key_shift};
  std::vector<Key*> right_wall_keys = {&key_5, &key_t, &key_g, &key_b};

  // Adjust the switch widths.
  for (Key* key : key_grid[0]) {
    key->extra_height_top = 8;
  }
  key_tilda.extra_height_bottom = 6;
  key_tilda.extra_width_right = 3;
  key_slash.extra_height_bottom = 8;
  key_left_arrow.extra_height_bottom = 8;
  key_slash.extra_width_left = 0;

  for (Key* key : left_wall_keys) {
    key->extra_width_left = 4;
  }
  for (Key* key : right_wall_keys) {
    key->extra_width_right = 4;
  }
  // This is shifted out a little to avoid hitting 4. Try to keep the right wall
  // straight
  key_5.extra_width_right = 3;
  for (Key* key : GetColumn(key_grid, 2)) {
    // s column
    key->extra_width_left = 4;
  }
  key_2.extra_width_left += 1.5;
  key_3.extra_width_left += 1;
  for (Key* key : GetColumn(key_grid, 1)) {
    // a column
    key->extra_width_left = 3;
  }
  key_tilda.extra_width_left = 4.5;
  key_shift.extra_height_bottom = 8;

  thumb.extra_height_bottom = 2;
  thumb.extra_width_left = 2;
  thumb_delete.extra_height_bottom = 2;
  thumb_end.extra_height_bottom = 2;

  thumb_ctrl.extra_width_left = 2;
  thumb_ctrl.extra_height_top = 2;
  thumb_alt.extra_height_top = 2;
  thumb_alt.extra_width_right = 2;
  thumb_alt.extra_width_left = 2;
  thumb_home.extra_width_right = 2;
  thumb_home.extra_width_left = 2;
  thumb_home.extra_height_top = 2;
  thumb_end.extra_height_top = 2;
  thumb_end.extra_width_right = 2;
  thumb_end.extra_width_left = 2;
  thumb_delete.extra_width_left = 2;

  for (Key* key : thumb_keys) {
    key->extra_z += 1;
  }
  key_5.extra_z += 1.5;

  // Keys are measured from the tip of the switch and keys are measured from the
  // tip of the cap. amount.
  double switch_top_z_offset = 10;
  for (Key* key : all_keys) {
    key->AddTransform();
    key->t().z -= 10;
    key->disable_switch_z_offset = true;
  }

  std::vector<Shape> shapes;
  for (Key* key : all_keys) {
    key->extra_z += 2;
    shapes.push_back(key->GetSwitch());
  }

  shapes.push_back(ConnectMainKeys(key_grid));
  shapes.push_back(ConnectHorizontal(thumb, thumb_delete));
  shapes.push_back(ConnectVertical(thumb_ctrl, thumb_delete));

  double wall_connector_offset = -1;
  Shape wall_connector = Cube(2, 2, 4).TranslateZ(-2);
  // The bottom left corner is a little messy. Add another point to try and
  // clean it up.
  TransformList extra_tilda_wall_point =
      key_shift.GetBottomRight(wall_connector_offset);
  {
    Transform& t = extra_tilda_wall_point.AddTransformFront();
    t.x = 2;
    t.y = 0;
  }

  // Also messy where main meets thumb
  TransformList extra_thumb_top_wall_point =
      thumb_ctrl.GetTopLeft(wall_connector_offset);
  {
    Transform& t = extra_thumb_top_wall_point.AddTransformFront();
    t.x = -6;
    t.z = -4;
  }

  std::vector<TransformList> wall_points = {
      key_shift.GetBottomRight(wall_connector_offset),
      key_shift.GetBottomLeft(wall_connector_offset),
      key_shift.GetTopLeft(wall_connector_offset),

      key_caps.GetBottomLeft(wall_connector_offset),
      key_caps.GetTopLeft(wall_connector_offset),

      key_tab.GetBottomLeft(wall_connector_offset),
      key_tab.GetTopLeft(wall_connector_offset),

      key_plus.GetBottomLeft(wall_connector_offset),
      key_plus.GetTopLeft(wall_connector_offset),
      key_plus.GetTopRight(wall_connector_offset),

      key_1.GetTopLeft(wall_connector_offset),
      key_1.GetTopRight(wall_connector_offset),

      key_2.GetTopLeft(wall_connector_offset),
      key_2.GetTopRight(wall_connector_offset),

      key_3.GetTopLeft(wall_connector_offset),
      key_3.GetTopRight(wall_connector_offset),

      key_4.GetTopLeft(wall_connector_offset),
      key_4.GetTopRight(wall_connector_offset),

      key_5.GetTopLeft(wall_connector_offset),
      key_5.GetTopRight(wall_connector_offset),
      key_5.GetBottomRight(wall_connector_offset),

      key_t.GetTopRight(wall_connector_offset),
      key_t.GetBottomRight(wall_connector_offset),

      key_g.GetTopRight(wall_connector_offset),
      key_g.GetBottomRight(wall_connector_offset),

      key_b.GetTopRight(wall_connector_offset),

      extra_thumb_top_wall_point,

      thumb_ctrl.GetTopLeft(wall_connector_offset),
      thumb_ctrl.GetTopRight(wall_connector_offset),

      thumb_alt.GetTopLeft(wall_connector_offset),
      thumb_alt.GetTopRight(wall_connector_offset),
      thumb_alt.GetBottomRight(wall_connector_offset),

      thumb_home.GetTopRight(wall_connector_offset),
      thumb_home.GetBottomRight(wall_connector_offset),

      thumb_end.GetTopRight(wall_connector_offset),
      thumb_end.GetBottomRight(wall_connector_offset),
      thumb_end.GetBottomLeft(wall_connector_offset),

      thumb_delete.GetBottomRight(wall_connector_offset),
      thumb_delete.GetBottomLeft(wall_connector_offset),

      thumb.GetBottomRight(wall_connector_offset),
      thumb.GetBottomLeft(wall_connector_offset),

      key_left_arrow.GetBottomRight(wall_connector_offset),
      key_left_arrow.GetBottomLeft(wall_connector_offset),

      key_slash.GetBottomRight(wall_connector_offset),
      key_slash.GetBottomLeft(wall_connector_offset),

      key_tilda.GetBottomRight(wall_connector_offset),
      key_tilda.GetBottomLeft(wall_connector_offset),
      key_tilda.GetTopLeft(wall_connector_offset),
  };

  auto make_wall = [=](const std::vector<TransformList>& points) {
    auto get_wall_post = [=](TransformList t) {
      Shape s = t.Apply(wall_connector);
      return Hull(s, s.Projection().LinearExtrude(.01));
    };
    std::vector<Shape> wall_shapes;
    for (size_t i = 0; i < points.size() - 1; ++i) {
      wall_shapes.push_back(
          Hull(get_wall_post(points[i]), get_wall_post(points[i + 1])));
    }
    return UnionAll(wall_shapes);
  };

  shapes.push_back(make_wall(wall_points));

  // Connect up thumb to main keys.
  shapes.push_back(Tri(
      extra_thumb_top_wall_point, thumb_ctrl.GetTopLeft(wall_connector_offset),
      thumb_delete.GetTopLeft(wall_connector_offset), wall_connector));
  {
    TransformList b_point = key_b.GetBottomRight();
    Transform& b_t = b_point.AddTransformFront();
    b_t.x = -2;
    b_t.z = -4;
    b_t.y = 1;

    TransformList a_point = key_right_arrow.GetTopRight(-1);
    Transform& a_t = a_point.AddTransformFront();
    a_t.x = 4;
    a_t.y = 2.5;

    shapes.push_back(Hull(
        thumb_delete.GetTopLeft(wall_connector_offset).Apply(wall_connector),
        b_point.Apply(wall_connector), a_point.Apply(wall_connector),
        thumb.GetTopLeft(wall_connector_offset).Apply(wall_connector),
        key_right_arrow.GetTopRight(0).Apply(wall_connector)));
  }

  {
    TransformList extra_thumb_wall_point =
        key_right_arrow.GetBottomRight(wall_connector_offset);
    Transform& t = extra_thumb_wall_point.AddTransformFront();
    t.x = -.5;
    t.y = 1;
    t.z = -3;
    shapes.push_back(
        Hull(thumb.GetBottomLeft(wall_connector_offset).Apply(wall_connector),
             extra_thumb_wall_point.Apply(wall_connector),
             key_right_arrow.GetBottomLeft(wall_connector_offset)
                 .Apply(wall_connector),
             key_left_arrow.GetBottomRight(wall_connector_offset)
                 .Apply(wall_connector)));
  }

  UnionAll(shapes).WriteToFile("dactyl.scad");
}

Shape ConnectMainKeys(const std::vector<std::vector<Key*>>& key_grid) {
  std::vector<Shape> shapes;
  for (int r = 0; r < kNumRows; ++r) {
    std::vector<Key*> keys = key_grid[r];
    bool has_top_keys = false;
    std::vector<Key*> top_keys;
    if (r > 0) {
      has_top_keys = true;
      top_keys = key_grid[r - 1];
    }
    for (int c = 0; c < kNumColumns; ++c) {
      Key* key = keys[c];
      if (key == nullptr) {
        // No key at this location.
        continue;
      }
      Key* left = nullptr;
      if (c > 0) {
        left = keys[c - 1];
      }

      Key* top_left = nullptr;
      Key* top = nullptr;
      if (has_top_keys) {
        top = top_keys[c];
        if (c > 0) {
          top_left = top_keys[c - 1];
        }
      }

      if (left != nullptr) {
        shapes.push_back(
            ConnectHorizontal(*left, *key, GetCapsuleConnector(), -1));
      }
      if (top != nullptr) {
        shapes.push_back(ConnectVertical(*top, *key));
        if (left != nullptr && top_left != nullptr) {
          shapes.push_back(ConnectDiagonal(*top_left, *top, *key, *left,
                                           GetCapsuleConnector(), -1));
        }
      }
    }
  }
  return UnionAll(shapes);
}

Key GetRotatedKey(double radius, bool up) {
  double distance = kBowlKeySpacing;
  double rotation_direction = up ? 1.0 : -1.0;
  double degrees = 1;

  // Use precomputed numbers for known radiuses to speed up execution.
  if (radius == 50) {
    degrees = 20.740;
  }
  if (radius == 55) {
    degrees = 18.840;
  }
  if (radius == 60) {
    degrees = 17.26;
  }
  if (radius == 65) {
    degrees = 15.920;
  }
  if (radius == 70) {
    degrees = 14.780;
  }

  bool is_first = true;
  while (true) {
    Key k;
    k.local_transforms.TranslateZ(-1 * radius)
        .RotateX(rotation_direction * degrees)
        .TranslateZ(radius);
    glm::vec3 point = k.GetTransforms().Apply(kOrigin);
    float current_distance = glm::length(point);
    if (current_distance > distance) {
      if (!is_first) {
        printf("Cumputed degrees %.3f for radius %.3f\n", degrees, radius);
      }
      return k;
    }
    degrees += .01;
    is_first = false;
  }
}
