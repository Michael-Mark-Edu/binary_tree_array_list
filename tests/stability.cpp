#include "../src/binary_tree_array_list.h"
#include <gtest/gtest.h>

using namespace imdast;

TEST(btal_stability_suite, left_balance_test) {
  auto list = binary_tree_array_list<int>();

  list.insert(0);
  list.insert(1);
  list.insert(2);

  ASSERT_EQ(list.size(), 3);
  ASSERT_EQ(list[0], 0);
  ASSERT_EQ(list[1], 1);
  ASSERT_EQ(list[2], 2);

  list.insert(3);
  list.insert(4);

  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], 0);
  ASSERT_EQ(list[1], 1);
  ASSERT_EQ(list[2], 2);
  ASSERT_EQ(list[3], 3);
  ASSERT_EQ(list[4], 4);

  list.insert(5);

  ASSERT_EQ(list.size(), 6);
  ASSERT_EQ(list[0], 0);
  ASSERT_EQ(list[1], 1);
  ASSERT_EQ(list[2], 2);
  ASSERT_EQ(list[3], 3);
  ASSERT_EQ(list[4], 4);
  ASSERT_EQ(list[5], 5);

  list.insert(6);
  ASSERT_EQ(list.size(), 7);
  ASSERT_EQ(list[0], 0);
  ASSERT_EQ(list[1], 1);
  ASSERT_EQ(list[2], 2);
  ASSERT_EQ(list[3], 3);
  ASSERT_EQ(list[4], 4);
  ASSERT_EQ(list[5], 5);
  ASSERT_EQ(list[6], 6);
}

TEST(btal_stability_suite, right_balance_test) {
  auto list = binary_tree_array_list<int>();

  list.insert(0);
  list.insert(-1);
  list.insert(-2);

  ASSERT_EQ(list.size(), 3);
  ASSERT_EQ(list[0], -2);
  ASSERT_EQ(list[1], -1);
  ASSERT_EQ(list[2], 0);

  list.insert(-3);
  list.insert(-4);

  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], -4);
  ASSERT_EQ(list[1], -3);
  ASSERT_EQ(list[2], -2);
  ASSERT_EQ(list[3], -1);
  ASSERT_EQ(list[4], 0);

  list.insert(-5);

  ASSERT_EQ(list.size(), 6);
  ASSERT_EQ(list[0], -5);
  ASSERT_EQ(list[1], -4);
  ASSERT_EQ(list[2], -3);
  ASSERT_EQ(list[3], -2);
  ASSERT_EQ(list[4], -1);
  ASSERT_EQ(list[5], 0);

  list.insert(-6);
  ASSERT_EQ(list.size(), 7);
  ASSERT_EQ(list[0], -6);
  ASSERT_EQ(list[1], -5);
  ASSERT_EQ(list[2], -4);
  ASSERT_EQ(list[3], -3);
  ASSERT_EQ(list[4], -2);
  ASSERT_EQ(list[5], -1);
  ASSERT_EQ(list[6], 0);
}

TEST(btal_stability_suite, left_right_balance_test) {
  auto list = binary_tree_array_list<int>();

  list.insert(5000);
  list.insert(2500);
  list.insert(3750);

  ASSERT_EQ(list.size(), 3);
  ASSERT_EQ(list[0], 2500);
  ASSERT_EQ(list[1], 3750);
  ASSERT_EQ(list[2], 5000);

  list.insert(1250);
  list.insert(1875);

  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 1875);
  ASSERT_EQ(list[2], 2500);
  ASSERT_EQ(list[3], 3750);
  ASSERT_EQ(list[4], 5000);

  list.insert(2187);

  ASSERT_EQ(list.size(), 6);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 1875);
  ASSERT_EQ(list[2], 2187);
  ASSERT_EQ(list[3], 2500);
  ASSERT_EQ(list[4], 3750);
  ASSERT_EQ(list[5], 5000);
}

TEST(btal_stability_suite, right_left_balance_test) {
  auto list = binary_tree_array_list<int>();

  list.insert(-5000);
  list.insert(-2500);
  list.insert(-3750);

  ASSERT_EQ(list.size(), 3);
  ASSERT_EQ(list[0], -5000);
  ASSERT_EQ(list[1], -3750);
  ASSERT_EQ(list[2], -2500);

  list.insert(-1250);
  list.insert(-1875);

  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], -5000);
  ASSERT_EQ(list[1], -3750);
  ASSERT_EQ(list[2], -2500);
  ASSERT_EQ(list[3], -1875);
  ASSERT_EQ(list[4], -1250);

  list.insert(-2187);

  ASSERT_EQ(list.size(), 6);
  ASSERT_EQ(list[0], -5000);
  ASSERT_EQ(list[1], -3750);
  ASSERT_EQ(list[2], -2500);
  ASSERT_EQ(list[3], -2187);
  ASSERT_EQ(list[4], -1875);
  ASSERT_EQ(list[5], -1250);
}

TEST(btal_stability_suite, complex_balance_test) {
  auto list = binary_tree_array_list<int>();

  // Left rotation
  list.insert(5000);
  list.insert(7500);
  list.insert(8750);
  ASSERT_EQ(list.size(), 3);
  ASSERT_EQ(list[0], 5000);
  ASSERT_EQ(list[1], 7500);
  ASSERT_EQ(list[2], 8750);

  // Right rotation
  list.insert(2500);
  list.insert(1250);
  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 2500);
  ASSERT_EQ(list[2], 5000);
  ASSERT_EQ(list[3], 7500);
  ASSERT_EQ(list[4], 8750);

  // Right-left rotation
  list.insert(9375);
  list.insert(9062);
  ASSERT_EQ(list.size(), 7);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 2500);
  ASSERT_EQ(list[2], 5000);
  ASSERT_EQ(list[3], 7500);
  ASSERT_EQ(list[4], 8750);
  ASSERT_EQ(list[5], 9062);
  ASSERT_EQ(list[6], 9375);

  // Left rotation
  list.insert(8906);
  list.insert(8984);
  ASSERT_EQ(list.size(), 9);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 2500);
  ASSERT_EQ(list[2], 5000);
  ASSERT_EQ(list[3], 7500);
  ASSERT_EQ(list[4], 8750);
  ASSERT_EQ(list[5], 8906);
  ASSERT_EQ(list[6], 8984);
  ASSERT_EQ(list[7], 9062);
  ASSERT_EQ(list[8], 9375);

  // Right rotation
  list.insert(3750);
  list.insert(3125);
  ASSERT_EQ(list.size(), 11);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 2500);
  ASSERT_EQ(list[2], 3125);
  ASSERT_EQ(list[3], 3750);
  ASSERT_EQ(list[4], 5000);
  ASSERT_EQ(list[5], 7500);
  ASSERT_EQ(list[6], 8750);
  ASSERT_EQ(list[7], 8906);
  ASSERT_EQ(list[8], 8984);
  ASSERT_EQ(list[9], 9062);
  ASSERT_EQ(list[10], 9375);

  // Right-left rotation
  list.insert(1875);
  list.insert(1562);
  ASSERT_EQ(list.size(), 13);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 1562);
  ASSERT_EQ(list[2], 1875);
  ASSERT_EQ(list[3], 2500);
  ASSERT_EQ(list[4], 3125);
  ASSERT_EQ(list[5], 3750);
  ASSERT_EQ(list[6], 5000);
  ASSERT_EQ(list[7], 7500);
  ASSERT_EQ(list[8], 8750);
  ASSERT_EQ(list[9], 8906);
  ASSERT_EQ(list[10], 8984);
  ASSERT_EQ(list[11], 9062);
  ASSERT_EQ(list[12], 9375);

  // Left-right rotation
  list.insert(9218);
  list.insert(9296);
  ASSERT_EQ(list.size(), 15);
  ASSERT_EQ(list[0], 1250);
  ASSERT_EQ(list[1], 1562);
  ASSERT_EQ(list[2], 1875);
  ASSERT_EQ(list[3], 2500);
  ASSERT_EQ(list[4], 3125);
  ASSERT_EQ(list[5], 3750);
  ASSERT_EQ(list[6], 5000);
  ASSERT_EQ(list[7], 7500);
  ASSERT_EQ(list[8], 8750);
  ASSERT_EQ(list[9], 8906);
  ASSERT_EQ(list[10], 8984);
  ASSERT_EQ(list[11], 9062);
  ASSERT_EQ(list[12], 9218);
  ASSERT_EQ(list[13], 9296);
  ASSERT_EQ(list[14], 9375);
}
