#pragma once

#include "key.h"
#include "transform.h"

namespace scad {

// Key positioning data and description of layout and grouping of keys.
struct KeyData {
  KeyData(TransformList origin);

  Key key_plus;
  Key key_1;
  Key key_2;
  Key key_3;
  Key key_4;
  Key key_5;

  Key key_tab;
  Key key_q;
  Key key_w;
  Key key_e;
  Key key_r;
  Key key_t;

  Key key_caps;
  Key key_a;
  Key key_s;
  Key key_d;
  Key key_f;
  Key key_g;

  Key key_shift;
  Key key_z;
  Key key_x;
  Key key_c;
  Key key_v;
  Key key_b;

  Key key_tilde;
  Key key_slash;
  Key key_left_arrow;
  Key key_right_arrow;

  Key key_backspace;
  Key key_delete;
  Key key_end;
  Key key_home;
  Key key_ctrl;
  Key key_alt;

  // clang-format off
  const std::vector<std::vector<Key*>> grid = {
    { &key_plus,   &key_1,      &key_2,      &key_3,           &key_4,            &key_5},
    { &key_tab,    &key_q,      &key_w,      &key_e,           &key_r,            &key_t},
    { &key_caps,   &key_a,      &key_s,      &key_d,           &key_f,            &key_g},
    { &key_shift,  &key_z,      &key_x,      &key_c,           &key_v,            &key_b},
    { nullptr,     &key_tilde,  &key_slash,  &key_left_arrow,  &key_right_arrow,  nullptr},
  };
  // clang-format on

  size_t num_columns() {
    return grid[0].size();
  }

  size_t num_rows() {
    return grid.size();
  }

  std::vector<Key*> column(int c) {
    std::vector<Key*> keys;
    for (auto& row : grid) {
      keys.push_back(row[c]);
    }
    return keys;
  }

  std::vector<Key*> row(int r) {
    return grid[r];
  }

  Key* get_key(int row, int column) {
    if (row < 0 || row >= num_rows() || column < 0 || column >= num_columns()) {
      return nullptr;
    }
    return grid[row][column];
  }

  std::vector<Key*> thumb_keys() {
    return {&key_delete, &key_backspace, &key_ctrl, &key_alt, &key_home, &key_end};
  }

  std::vector<Key*> grid_keys() {
    std::vector<Key*> keys;
    for (auto& row : grid) {
      for (Key* key : row) {
        if (key != nullptr) {
          keys.push_back(key);
        }
      }
    }
    return keys;
  }

  std::vector<Key*> all_keys() {
    std::vector<Key*> keys;
    for (Key* key : thumb_keys()) {
      keys.push_back(key);
    }
    for (Key* key : grid_keys()) {
      keys.push_back(key);
    }
    return keys;
  }
};

}  // namespace scad
