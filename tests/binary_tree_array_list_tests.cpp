#include "../src/binary_tree_array_list.h"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace imdast;

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

TEST(btal_functions_suite, begin_end_test) {
  auto list = binary_tree_array_list<int>();

  EXPECT_EQ(list.begin(), list.end());
  list.insert(5);
  EXPECT_NE(list.begin(), list.end());
  list.insert(8);
  EXPECT_NE(list.begin(), list.end());
}

TEST(btal_functions_suite, iterator_move_test) {
  auto list = binary_tree_array_list<int>();

  auto iter = list.begin();
  EXPECT_FALSE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_FALSE(iter.next());
  EXPECT_FALSE(iter.prev());

  list.insert(5);
  iter = list.begin();

  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_FALSE(iter.prev());
  EXPECT_NE(iter, list.end());

  EXPECT_TRUE(iter.next());
  EXPECT_FALSE(iter.has_next());
  EXPECT_TRUE(iter.has_prev());
  EXPECT_FALSE(iter.next());
  EXPECT_EQ(iter, list.end());

  EXPECT_TRUE(iter.prev());
  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_FALSE(iter.prev());
  EXPECT_NE(iter, list.end());

  list.insert(8);
  iter = list.begin();

  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_FALSE(iter.prev());
  EXPECT_NE(iter, list.end());

  EXPECT_TRUE(iter.next());
  EXPECT_TRUE(iter.has_next());
  EXPECT_TRUE(iter.has_prev());
  EXPECT_NE(iter, list.end());

  EXPECT_TRUE(iter.next());
  EXPECT_FALSE(iter.has_next());
  EXPECT_TRUE(iter.has_prev());
  EXPECT_FALSE(iter.next());
  EXPECT_EQ(iter, list.end());

  EXPECT_TRUE(iter.prev());
  EXPECT_TRUE(iter.has_next());
  EXPECT_TRUE(iter.has_prev());
  EXPECT_NE(iter, list.end());

  EXPECT_TRUE(iter.prev());
  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_FALSE(iter.prev());
  EXPECT_NE(iter, list.end());
}

TEST(btal_functions_suite, iterator_index_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(40);
  list.insert(-5);
  list.insert(25);
  list.insert(80);

  auto iter = list.begin();

  EXPECT_EQ(iter.index(), 0);
  iter.next();
  EXPECT_EQ(iter.index(), 1);
  iter.next();
  EXPECT_EQ(iter.index(), 2);
  iter.next();
  EXPECT_EQ(iter.index(), 3);
  iter.next();
  EXPECT_EQ(iter.index(), 4);
  iter.next();
  EXPECT_EQ(iter.index(), 4);
  iter.prev();
  EXPECT_EQ(iter.index(), 3);
  iter.prev();
  EXPECT_EQ(iter.index(), 2);
  iter.prev();
  EXPECT_EQ(iter.index(), 1);
  iter.prev();
  EXPECT_EQ(iter.index(), 0);
}

TEST(btal_functions_suite, iterator_get_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(40);
  list.insert(-5);
  list.insert(25);
  list.insert(80);

  auto iter = list.begin();

  EXPECT_EQ(*iter.get(), -5);
  EXPECT_EQ(*iter, -5);
  iter.next();
  EXPECT_EQ(*iter.get(), 25);
  EXPECT_EQ(*iter, 25);
  iter.next();
  EXPECT_EQ(*iter.get(), 40);
  EXPECT_EQ(*iter, 40);
  iter.next();
  EXPECT_EQ(*iter.get(), 80);
  EXPECT_EQ(*iter, 80);
  iter.next();
  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_THROW(*iter, std::logic_error);
  iter.prev();
  EXPECT_EQ(*iter.get(), 80);
  EXPECT_EQ(*iter, 80);
  iter.prev();
  EXPECT_EQ(*iter.get(), 40);
  EXPECT_EQ(*iter, 40);
  iter.prev();
  EXPECT_EQ(*iter.get(), 25);
  EXPECT_EQ(*iter, 25);
  iter.prev();
  EXPECT_EQ(*iter.get(), -5);
  EXPECT_EQ(*iter, -5);
}

TEST(btal_functions_suite, for_loop_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(40);
  list.insert(-5);
  list.insert(25);
  list.insert(80);

  int i = 0;
  int a[] = {-5, 25, 40, 80};
  for (int item : list)
    EXPECT_EQ(item, a[i++]);
}

TEST(btal_functions_suite, begin_at_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(40);
  list.insert(-5);
  list.insert(25);
  list.insert(80);

  auto iter = list.begin_at(0);
  EXPECT_EQ(*iter, -5);
  EXPECT_EQ(iter.index(), 0);

  iter = list.begin_at(1);
  EXPECT_EQ(*iter, 25);
  EXPECT_EQ(iter.index(), 1);

  iter = list.begin_at(2);
  EXPECT_EQ(*iter, 40);
  EXPECT_EQ(iter.index(), 2);

  iter = list.begin_at(3);
  EXPECT_EQ(*iter, 80);
  EXPECT_EQ(iter.index(), 3);

  iter = list.begin_at(4);
  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_EQ(iter.index(), 4);

  iter = list.begin_at(5);
  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_EQ(iter.index(), 4);
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

TEST(btal_functions_suite, copy_constructor_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(21);
  list.insert(8);

  binary_tree_array_list<int> list2(list);

  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list2.size(), 2);
  EXPECT_EQ(list[0], 8);
  EXPECT_EQ(list[1], 21);
  EXPECT_EQ(list2[0], 8);
  EXPECT_EQ(list2[1], 21);

  list.insert(12);
  list2.insert(16);
  list2.insert(25);

  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list2.size(), 4);
  EXPECT_EQ(list[0], 8);
  EXPECT_EQ(list[1], 12);
  EXPECT_EQ(list[2], 21);
  EXPECT_EQ(list2[0], 8);
  EXPECT_EQ(list2[1], 16);
  EXPECT_EQ(list2[2], 21);
  EXPECT_EQ(list2[3], 25);
}

TEST(btal_functions_suite, iterator_copy_constructor_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(21);
  list.insert(8);

  binary_tree_array_list<int>::iterator iter = list.begin_at(1);
  binary_tree_array_list<int>::iterator iter2(iter);

  EXPECT_EQ(*iter, 21);
  EXPECT_EQ(iter.index(), 1);
  EXPECT_TRUE(iter.has_next());
  EXPECT_TRUE(iter.has_prev());
  EXPECT_EQ(*iter2, 21);
  EXPECT_EQ(iter2.index(), 1);
  EXPECT_TRUE(iter2.has_next());
  EXPECT_TRUE(iter2.has_prev());

  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter, 8);
  EXPECT_EQ(iter.index(), 0);
  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_EQ(*iter2, 21);
  EXPECT_EQ(iter2.index(), 1);
  EXPECT_TRUE(iter2.has_next());
  EXPECT_TRUE(iter2.has_prev());

  EXPECT_TRUE(iter2.next());
  EXPECT_EQ(*iter, 8);
  EXPECT_EQ(iter.index(), 0);
  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_EQ(iter2.get(), nullptr);
  EXPECT_EQ(iter2.index(), 2);
  EXPECT_FALSE(iter2.has_next());
  EXPECT_TRUE(iter2.has_prev());
}

TEST(btal_functions_suite, operator_assign_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(21);
  list.insert(8);

  binary_tree_array_list<int> list2;
  list2 = list;

  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list2.size(), 2);
  EXPECT_EQ(list[0], 8);
  EXPECT_EQ(list[1], 21);
  EXPECT_EQ(list2[0], 8);
  EXPECT_EQ(list2[1], 21);

  list.insert(12);
  list2.insert(16);
  list2.insert(25);

  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list2.size(), 4);
  EXPECT_EQ(list[0], 8);
  EXPECT_EQ(list[1], 12);
  EXPECT_EQ(list[2], 21);
  EXPECT_EQ(list2[0], 8);
  EXPECT_EQ(list2[1], 16);
  EXPECT_EQ(list2[2], 21);
  EXPECT_EQ(list2[3], 25);
}

TEST(btal_functions_suite, iterator_operator_assign_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(21);
  list.insert(8);

  binary_tree_array_list<int>::iterator iter = list.begin_at(1);
  binary_tree_array_list<int>::iterator iter2;
  iter2 = iter;

  EXPECT_EQ(*iter, 21);
  EXPECT_EQ(iter.index(), 1);
  EXPECT_TRUE(iter.has_next());
  EXPECT_TRUE(iter.has_prev());
  EXPECT_EQ(*iter2, 21);
  EXPECT_EQ(iter2.index(), 1);
  EXPECT_TRUE(iter2.has_next());
  EXPECT_TRUE(iter2.has_prev());

  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter, 8);
  EXPECT_EQ(iter.index(), 0);
  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_EQ(*iter2, 21);
  EXPECT_EQ(iter2.index(), 1);
  EXPECT_TRUE(iter2.has_next());
  EXPECT_TRUE(iter2.has_prev());

  EXPECT_TRUE(iter2.next());
  EXPECT_EQ(*iter, 8);
  EXPECT_EQ(iter.index(), 0);
  EXPECT_TRUE(iter.has_next());
  EXPECT_FALSE(iter.has_prev());
  EXPECT_EQ(iter2.get(), nullptr);
  EXPECT_EQ(iter2.index(), 2);
  EXPECT_FALSE(iter2.has_next());
  EXPECT_TRUE(iter2.has_prev());
}

TEST(btal_functions_suite, iterator_default_constructor_test) {
  binary_tree_array_list<int>::iterator iter;

  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_THROW(*iter, std::logic_error);
  EXPECT_FALSE(iter.has_prev());
  EXPECT_FALSE(iter.prev());
  EXPECT_FALSE(iter.has_next());
  EXPECT_FALSE(iter.next());
  EXPECT_EQ(iter.index(), 0);

  binary_tree_array_list<int>::iterator iter2;
  EXPECT_EQ(iter, iter2);
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
