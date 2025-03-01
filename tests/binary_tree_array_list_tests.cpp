#include "../src/binary_tree_array_list.h"
#include <gtest/gtest.h>
#include <stdexcept>

TEST(btal_functions_suite, default_constructor_test) {
  auto ints = binary_tree_array_list<int>();
  auto pointers = binary_tree_array_list<int *>();
  auto unique = binary_tree_array_list<std::unique_ptr<int>>();
}

TEST(btal_functions_suite, insert_test) {
  auto list = binary_tree_array_list<int>();

  for (int i = 0; i < 10'000; i++) {
    ASSERT_NO_THROW(list.insert(i));
  }
  EXPECT_NO_THROW(list.insert(10'000));
  EXPECT_NO_THROW(list.insert(10'000));
}

TEST(btal_functions_suite, size_test) {
  auto list = binary_tree_array_list<int>();

  EXPECT_EQ(list.size(), 0);
  for (int i = 0; i < 10'000; i++) {
    list.insert(i);
    ASSERT_EQ(list.size(), i + 1);
  }
  list.insert(10'000);
  EXPECT_EQ(list.size(), 10'001);
  list.insert(10'000);
  EXPECT_EQ(list.size(), 10'002);
}

TEST(btal_functions_suite, capacity_test) {
  auto list = binary_tree_array_list<int>();

  EXPECT_GE(list.capacity(), 0);
  for (int i = 0; i < 10'000; i++) {
    list.insert(i);
    ASSERT_GE(list.size(), i + 1);
  }
  list.insert(10'000);
  EXPECT_GE(list.size(), 10'001);
  list.insert(10'000);
  EXPECT_GE(list.size(), 10'002);
}

TEST(btal_functions_suite, clear_test) {
  auto list = binary_tree_array_list<int>();

  list.insert(0);
  size_t capacity = list.capacity();
  list.clear();
  EXPECT_EQ(list.size(), 0);
  EXPECT_EQ(list.capacity(), capacity);
}

TEST(btal_functions_suite, empty_test) {
  auto list = binary_tree_array_list<int>();

  EXPECT_TRUE(list.empty());
  list.insert(0);
  EXPECT_FALSE(list.empty());
  list.insert(1);
  EXPECT_FALSE(list.empty());
}

TEST(btal_functions_suite, get_test) {
  auto list = binary_tree_array_list<int>();

  list.insert(21);
  list.insert(8);

  EXPECT_EQ(*list.get(0), 8);
  EXPECT_EQ(*list.get(1), 21);
  EXPECT_EQ(list.get(2), nullptr);
  EXPECT_EQ(list.get(-1), nullptr);
}

TEST(btal_functions_suite, subscript_test) {
  auto list = binary_tree_array_list<int>();

  list.insert(21);
  list.insert(8);

  EXPECT_EQ(list[0], 8);
  EXPECT_EQ(list[1], 21);
  EXPECT_THROW(list[2], std::logic_error);
  EXPECT_THROW(list[-1], std::logic_error);
}

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
