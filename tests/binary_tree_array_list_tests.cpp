#include "../src/binary_tree_array_list.h"
#include <gtest/gtest.h>
#include <optional>
#include <stdexcept>

TEST(binary_tree_array_list_suite, empty_list_test) {
  auto list = binary_tree_array_list<int>();
  EXPECT_EQ(list.size(), 0);
  EXPECT_EQ(list.capacity(), 0);

  binary_tree_array_list<int>::iterator iter = list.begin();
  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_FALSE(iter.next());
}

TEST(binary_tree_array_list_suite, insert_in_order_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(1);
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.capacity(), 1);
  list.insert(2);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.capacity(), 3);
  list.insert(3);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.capacity(), 7);

  EXPECT_EQ(list.get_raw(0), std::make_optional(1));
  EXPECT_EQ(list.get_raw(2), std::make_optional(2));
  EXPECT_EQ(list.get_raw(6), std::make_optional(3));

  EXPECT_EQ(*list.get(0), 1);
  EXPECT_EQ(*list.get(1), 2);
  EXPECT_EQ(*list.get(2), 3);
  EXPECT_EQ(list.get(3), nullptr);

  EXPECT_EQ(list[0], 1);
  EXPECT_EQ(list[1], 2);
  EXPECT_EQ(list[2], 3);
  EXPECT_THROW(list[3], std::logic_error);

  binary_tree_array_list<int>::iterator iter = list.begin();
  EXPECT_EQ(*iter.get(), 1);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 2);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 3);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_FALSE(iter.next());
}

TEST(binary_tree_array_list_suite, insert_in_reverse_order_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(3);
  list.insert(2);
  list.insert(1);

  EXPECT_EQ(list.get_raw(3), std::make_optional(1));
  EXPECT_EQ(list.get_raw(1), std::make_optional(2));
  EXPECT_EQ(list.get_raw(0), std::make_optional(3));
}

TEST(binary_tree_array_list_suite, insert_in_random_order_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(5);
  list.insert(9);
  list.insert(4);
  list.insert(0);
  list.insert(8);
  list.insert(1);
  list.insert(2);
  list.insert(6);
  list.insert(3);
  list.insert(7);
  EXPECT_EQ(list.size(), 10);

  EXPECT_EQ(list.get_raw(0), std::make_optional(5));
  EXPECT_EQ(list.get_raw(1), std::make_optional(4));
  EXPECT_EQ(list.get_raw(2), std::make_optional(9));
  EXPECT_EQ(list.get_raw(3), std::make_optional(0));
  EXPECT_EQ(list.get_raw(5), std::make_optional(8));
  EXPECT_EQ(list.get_raw(8), std::make_optional(1));
  EXPECT_EQ(list.get_raw(11), std::make_optional(6));
  EXPECT_EQ(list.get_raw(18), std::make_optional(2));
  EXPECT_EQ(list.get_raw(24), std::make_optional(7));
  EXPECT_EQ(list.get_raw(38), std::make_optional(3));

  EXPECT_EQ(list[0], 0);
  EXPECT_EQ(list[1], 1);
  EXPECT_EQ(list[2], 2);
  EXPECT_EQ(list[3], 3);
  EXPECT_EQ(list[4], 4);
  EXPECT_EQ(list[5], 5);
  EXPECT_EQ(list[6], 6);
  EXPECT_EQ(list[7], 7);
  EXPECT_EQ(list[8], 8);
  EXPECT_EQ(list[9], 9);

  EXPECT_EQ(*list.get(0), 0);
  EXPECT_EQ(*list.get(1), 1);
  EXPECT_EQ(*list.get(2), 2);
  EXPECT_EQ(*list.get(3), 3);
  EXPECT_EQ(*list.get(4), 4);
  EXPECT_EQ(*list.get(5), 5);
  EXPECT_EQ(*list.get(6), 6);
  EXPECT_EQ(*list.get(7), 7);
  EXPECT_EQ(*list.get(8), 8);
  EXPECT_EQ(*list.get(9), 9);

  binary_tree_array_list<int>::iterator iter = list.begin();
  EXPECT_EQ(*iter.get(), 0);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 1);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 2);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 3);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 4);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 5);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 6);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 7);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 8);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 9);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_FALSE(iter.next());
}

TEST(binary_tree_array_list_suite, indexed_iterator_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(5);
  list.insert(9);
  list.insert(4);
  list.insert(0);
  list.insert(8);
  list.insert(1);
  list.insert(2);
  list.insert(6);
  list.insert(3);
  list.insert(7);

  binary_tree_array_list<int>::iterator iter = list.begin_at(5);

  EXPECT_EQ(*iter.get(), 5);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 6);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 7);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 8);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(*iter.get(), 9);
  EXPECT_TRUE(iter.next());
  EXPECT_EQ(iter.get(), nullptr);
  EXPECT_FALSE(iter.next());
}

TEST(binary_tree_array_list_suite, prev_iterator_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(5);
  list.insert(9);
  list.insert(4);
  list.insert(0);
  list.insert(8);
  list.insert(1);
  list.insert(2);
  list.insert(6);
  list.insert(3);
  list.insert(7);

  binary_tree_array_list<int>::iterator iter = list.begin_at(9);
  EXPECT_EQ(*iter.get(), 9);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 8);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 7);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 6);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 5);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 4);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 3);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 2);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 1);
  EXPECT_TRUE(iter.prev());
  EXPECT_EQ(*iter.get(), 0);
  EXPECT_FALSE(iter.prev());
  EXPECT_EQ(*iter.get(), 0);
}

TEST(binary_tree_array_list_suite, range_for_test) {
  auto list = binary_tree_array_list<int>();
  list.insert(5);
  list.insert(9);
  list.insert(4);
  list.insert(0);
  list.insert(8);
  list.insert(1);
  list.insert(2);
  list.insert(6);
  list.insert(3);
  list.insert(7);

  size_t index = 0;
  for (int item : list) {
    EXPECT_EQ(item, index++);
  }
}
