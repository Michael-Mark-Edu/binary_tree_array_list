/* Copyright 2025 Michael Mark
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef IMDAST_BINARY_TREE_ARRAY_LIST_H
#define IMDAST_BINARY_TREE_ARRAY_LIST_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <limits>
#include <optional>
#include <stack>
#include <stdexcept>
#include <utility>

#define LEFT(n) ((n) * 2 + 1)
#define RIGHT(n) ((n) * 2 + 2)
#define PARENT(n) (((n) - 1) / 2)

namespace imdast {
template <class T> class binary_tree_array_list {
  std::optional<T> *_data;
  uint8_t *_height;
  size_t _size;
  size_t _capacity;

  void shift(size_t current, long long shift_amount) {
    if (current >= _capacity || !_data[current].has_value() ||
        shift_amount == 0)
      return;

    if (shift_amount > 0) {
      shift(LEFT(current), shift_amount * 2);
      shift(RIGHT(current), shift_amount * 2);
    }

    _data[current + shift_amount] = std::move(_data[current]);
    _height[current + shift_amount] = std::move(_height[current]);
    _data[current].reset();
    _height[current] = 0;

    if (shift_amount < 0) {
      shift(LEFT(current), shift_amount * 2);
      shift(RIGHT(current), shift_amount * 2);
    }
  }

  void deep_copy(const binary_tree_array_list<T> &list) {
    this->_size = list._size;
    this->_capacity = list._capacity;
    this->_data = static_cast<std::optional<T> *>(
        malloc(list._capacity * sizeof(std::optional<T>)));
    std::memcpy(this->_data, list._data,
                list._capacity * sizeof(std::optional<T>));
    this->_height =
        static_cast<uint8_t *>(malloc(list._capacity * sizeof(uint8_t)));
    std::memcpy(this->_height, list._height, list._capacity * sizeof(uint8_t));
  }

  void rebalance(size_t x) {
    uint8_t rotscore = 0;
    size_t y;
    if (_height[LEFT(x)] > _height[RIGHT(x)]) {
      rotscore += 0;
      y = LEFT(x);
    } else {
      rotscore += 1;
      y = RIGHT(x);
    }

    size_t z;
    if (_height[LEFT(y)] > _height[RIGHT(y)]) {
      rotscore += 0;
      z = LEFT(y);
    } else {
      rotscore += 2;
      z = RIGHT(y);
    }

    switch (rotscore) {
    // Rotate right
    case 0:
      std::swap(_data[x], _data[y]);
      shift(RIGHT(x), RIGHT(RIGHT(x)) - RIGHT(x));
      _data[RIGHT(x)] = std::move(_data[y]);
      shift(RIGHT(y), 1);
      shift(z, y - z);
      break;

    // Rotate right-left
    case 1:
      shift(LEFT(x), LEFT(LEFT(x)) - LEFT(x));
      _data[LEFT(x)] = std::move(_data[x]);
      _data[x] = std::move(_data[z]);
      _data[z].reset();
      _height[z] = 0;
      shift(LEFT(z), RIGHT(LEFT(x)) - LEFT(z));
      shift(RIGHT(z), z - RIGHT(z));
      break;

    // Rotate left-right
    case 2:
      shift(RIGHT(x), RIGHT(RIGHT(x)) - RIGHT(x));
      _data[RIGHT(x)] = std::move(_data[x]);
      _data[x] = std::move(_data[z]);
      _data[z].reset();
      _height[z] = 0;
      shift(RIGHT(z), LEFT(RIGHT(x)) - RIGHT(z));
      shift(LEFT(z), z - LEFT(z));
      break;

    // Rotate left
    case 3:
      std::swap(_data[x], _data[y]);
      shift(LEFT(x), LEFT(LEFT(x)) - LEFT(x));
      _data[LEFT(x)] = std::move(_data[y]);
      shift(LEFT(y), -1);
      shift(z, y - z);
      break;
    }
    _height[LEFT(x)] =
        std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
    _height[RIGHT(x)] =
        std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
    _height[x] = std::max(_height[RIGHT(x)], _height[LEFT(x)]) + 1;
  }

public:
  class iterator {
    const binary_tree_array_list<T> *_list;
    size_t _current;

    void construct_at_zero() noexcept {
      if (_list->_size == 0) {
        _current = std::numeric_limits<size_t>::max();
        return;
      }
      _current = 0;
      while (LEFT(_current) < _list->_capacity) {
        if (!_list->_data[LEFT(_current)].has_value()) {
          return;
        }
        _current = LEFT(_current);
      }
    }

    // Parameters are reversed compared to how I usually put them in order to
    // disambiguate the iterator. It's ugly, but works well enough for a private
    // API.
    iterator(size_t current, const binary_tree_array_list<T> *list)
        : _list(list), _current(current) {}

  public:
    // Creates an iterator with no associated list.
    iterator() noexcept
        : _list(nullptr), _current(std::numeric_limits<size_t>::max()) {}

    // Creates an iterator pointing to the smallest item in the list.
    iterator(const binary_tree_array_list<T> *list) noexcept : _list(list) {
      construct_at_zero();
    }

    // Creates an iterator pointing to the nth (0-indexed) smallest item in the
    // list. If index is >= list->size(), then the iterator will point to the
    // greatest item in the list.
    iterator(const binary_tree_array_list<T> *list, size_t index) noexcept
        : _list(list) {
      construct_at_zero();
      for (size_t i = 0; i < index; i++) {
        this->next();
      }
    }

    // Performs a shallow copy of the iterator. The copy will act independently
    // from the original iterator.
    iterator(const binary_tree_array_list<T>::iterator &iter) noexcept
        : _list(iter._list), _current(iter._current) {}

    // Searches for the item, then constructs an iterator starting at that item.
    // If the list does not contain the item, then the iterator will start at
    // the past-the-last element.
    static iterator find(const binary_tree_array_list<T> *list,
                         const T &item) noexcept {
      size_t current = 0;
      while (current < list->_capacity && list->_data[current].has_value()) {
        if (item == list->_data[current].value()) {
          return iterator(current, list);
        }
        current = item < list->_data[current].value() ? LEFT(current)
                                                      : RIGHT(current);
      }
      current = -1;
      return iterator(current, list);
    }

    // Returns an optional by-value to the current item. May be nullopt.
    std::optional<T> get() const noexcept {
      if (!_list || _current == std::numeric_limits<size_t>::max())
        return std::nullopt;
      else
        return _list->_data[_current];
    }

    // Checks if calling next() would pass or fail.
    bool has_next() const noexcept {
      return _current != std::numeric_limits<size_t>::max();
    }

    // Checks if calling prev() would pass or fail.
    bool has_prev() const noexcept {
      if (!_list)
        return false;
      if (_current == std::numeric_limits<size_t>::max())
        return _list->_size > 0;
      if (LEFT(_current) < _list->_capacity &&
          _list->_data[LEFT(_current)].has_value())
        return true;
      size_t index = _current;
      while (index > 0) {
        if (_list->_data[PARENT(index)].value() <
            _list->_data[_current].value())
          return true;
        index = PARENT(index);
      }
      return false;
    }

    // Moves the iterator to the next item in the list. Returns whether the
    // iterator actually moved. Final item is nullptr.
    bool next() noexcept {
      if (!_list || _current == std::numeric_limits<size_t>::max())
        return false;
      size_t offset = _current;
      if (RIGHT(offset) >= _list->_capacity ||
          !_list->_data[RIGHT(offset)].has_value()) {
        T value = _list->_data[_current].value();
        size_t parent = offset;
        do {
          if (parent == 0) {
            _current = std::numeric_limits<size_t>::max();
            return true;
          }
          parent = PARENT(parent);
        } while (_list->_data[parent].value() < value);
        _current = parent;
        return true;
      }
      offset = RIGHT(offset);
      while (LEFT(offset) < _list->_capacity &&
             _list->_data[LEFT(offset)].has_value()) {
        offset = LEFT(offset);
      }
      _current = offset;
      return true;
    }

    // Moves the iterator to the previous item in the list. Returns whether the
    // iterator actually moved. Final item is the first item.
    bool prev() noexcept {
      if (!_list || _list->_size == 0 ||
          (_current == std::numeric_limits<size_t>::max() && _list->_size == 0))
        return false;

      {
        size_t i = 0;
        while (LEFT(i) < _list->_capacity &&
               _list->_data[LEFT(i)].has_value()) {
          i = LEFT(i);
        }
        if (_current == i)
          return false;
      }

      if (_current == std::numeric_limits<size_t>::max()) {
        _current = 0;
        while (RIGHT(_current) < _list->_capacity &&
               _list->_data[RIGHT(_current)].has_value()) {
          _current = RIGHT(_current);
        }
        return true;
      }
      size_t offset = _current;
      if (LEFT(offset) >= _list->_capacity ||
          !_list->_data[LEFT(offset)].has_value()) {
        T value = _list->_data[_current].value();
        size_t parent = offset;
        do {
          if (parent == 0) {
            _current = std::numeric_limits<size_t>::max();
            return true;
          }
          parent = PARENT(parent);
        } while (_list->_data[parent].value() > value);
        _current = parent;
        return true;
      }
      offset = LEFT(offset);
      while (RIGHT(offset) < _list->_capacity &&
             _list->_data[RIGHT(offset)].has_value()) {
        offset = RIGHT(offset);
      }
      _current = offset;
      return true;
    }

    // Returns the value by-value at the iterator's current position. Throws a
    // std::logic_error if called on the past-the-last item. If this behavior is
    // not desired, then use the get() method instead.
    T operator*() const {
      std::optional<T> item = get();
      return item.has_value() ? item.value()
                              : throw std::logic_error(
                                    "Tried to dereference past-the-last item");
    }

    // Tests if two iterators are identical.
    bool operator==(binary_tree_array_list::iterator iter) const noexcept {
      return _list == iter._list && _current == iter._current;
    }

    // Tests if two iterators are not identical.
    bool operator!=(binary_tree_array_list::iterator iter) const noexcept {
      return _list != iter._list || _current != iter._current;
    }

    // Moves the iterator to the next item in the list. Returns a reference to
    // this iterator.
    binary_tree_array_list::iterator &operator++() noexcept {
      next();
      return *this;
    }

    // Shallow-copies the right iterator into the left.
    binary_tree_array_list<T>::iterator &
    operator=(const binary_tree_array_list<T>::iterator &right) {
      this->_list = right._list;
      this->_current = right._current;
      return *this;
    }
  }; // class iterator

  // Creates an empty binary tree array list.
  binary_tree_array_list() noexcept
      : _data(nullptr), _height(nullptr), _size(0), _capacity(0) {}

  // Creates a deep copy of the list.
  binary_tree_array_list(const binary_tree_array_list<T> &list) noexcept {
    deep_copy(list);
  }

  ~binary_tree_array_list() noexcept {
    free(_data);
    free(_height);
  }

  // Returns the number of items in the list.
  size_t size() const noexcept { return _size; }

  // Returns the maximum number of items the list can contain without needing to
  // reallocate.
  size_t capacity() const noexcept { return _capacity; }

  // Returns if the list is empty.
  bool empty() const noexcept { return !_size; }

  // Removes all items from the list. Does not shrink the list's allocation.
  void clear() {
    free(_data);
    free(_height);
    _data = static_cast<std::optional<T> *>(
        malloc(_capacity * sizeof(std::optional<T>)));
    for (size_t i = 0; i < _capacity; i++) {
      _data[i] = std::optional<T>();
    }
    _height = static_cast<uint8_t *>(calloc(_capacity, sizeof(uint8_t)));
    _size = 0;
  }

  // Inserts a value into the list in-order.
  void insert(const T &value) {
    size_t index = 0;
    while (true) {
      if (index >= _capacity) {
        size_t old_capacity = _capacity;
        _capacity = LEFT(_capacity);
        _data = static_cast<std::optional<T> *>(
            realloc(_data, _capacity * sizeof(std::optional<T>)));
        _height = static_cast<uint8_t *>(
            realloc(_height, _capacity * sizeof(uint8_t)));
        for (size_t i = old_capacity; i < _capacity; i++) {
          _data[i] = std::optional<T>();
          _height[i] = 0;
        }
      }
      if (!_data[index].has_value()) {
        _data[index] = std::make_optional(value);
        _size++;
        break;
      }
      index = LEFT(index) + (_data[index].value() < value);
    }

    _height[index] = 1;
    while (index > 0) {
      index = PARENT(index);
      if (std::abs(_height[RIGHT(index)] - _height[LEFT(index)]) >= 2) {
        rebalance(index);
      }
      _height[index] =
          std::max(_height[LEFT(index)], _height[RIGHT(index)]) + 1;
    }
  }

  // Removes an item from the list, returning whether said item was in the list.
  bool remove(const T &value) {
    size_t index = 0;
    while (index < _capacity && _data[index].has_value()) {
      if (_data[index].value() == value) {
        goto found;
      }
      index = value < _data[index] ? LEFT(index) : RIGHT(index);
    }
    return false;

  found:
    size_t next = RIGHT(index);
    if (next >= _capacity || !_data[next].has_value()) {
      _data[index].reset();
    } else {
      while (LEFT(next) < _capacity && _data[LEFT(next)].has_value()) {
        next = LEFT(next);
      }
      _data[index] = std::move(_data[next]);
      if (RIGHT(next) >= _capacity || !_data[RIGHT(next)].has_value()) {
        _data[next].reset();
      } else {
        shift(RIGHT(next), next - RIGHT(next));
      }
    }

    while (index > 0) {
      index = PARENT(index);
      if (std::abs(_height[RIGHT(index)] - _height[LEFT(index)]) >= 2) {
        rebalance(index);
      }
      _height[index] =
          std::max(_height[LEFT(index)], _height[RIGHT(index)]) + 1;
    }

    _size--;
    return true;
  }

  // Checks if the list contains an item.
  bool contains(const T &value) const noexcept {
    size_t index = 0;
    while (index < _capacity && _data[index].has_value()) {
      if (value == _data[index].value())
        return true;
      index = value < _data[index].value() ? LEFT(index) : RIGHT(index);
    }
    return false;
  }

  // Returns an iterator starting at where the given value is, if the list
  // contains that value. Otherwise, returns an iterator to the past-the-last
  // item.
  iterator find(const T &value) const noexcept {
    return iterator::find(this, value);
  }

  // Returns an optional by-value to the nth (0-indexed) item in the list.
  // Unlike the [] operator, this method cannot throw an exception if index is
  // too large.
  std::optional<T> get(size_t index) const noexcept {
    iterator iter = iterator(this);
    for (size_t i = 0; i < index; i++) {
      if (!iter.next())
        return std::nullopt;
    }
    return iter.get();
  }

  // Returns by-value the nth (0-indexed) item in the list. If index is >=
  // size(), then a std::logic_error is thrown. If this behavior is not desired,
  // then use the get() method instead.
  T operator[](size_t index) const {
    if (index >= _size)
      throw std::logic_error("Subscript out-of-bounds");
    iterator iter = iterator(this);
    for (size_t i = 0; i < index; i++) {
      iter.next();
    }
    return iter.get().value();
  }

  // Creates an iterator pointing to the smallest item in the list.
  iterator begin() const noexcept { return iterator(this); }

  // Creates an iterator pointing to the nth (0-indexed) smallest item in the
  // list. If index is >= list->size(), then the iterator will point to the
  // greatest item in the list.
  iterator begin_at(size_t index) const noexcept {
    return iterator(this, index);
  }

  // Creates an iterator pointing to the past-the-last item.
  iterator end() const noexcept { return iterator(this, _size); }

  // Deep-copies the right list into the left.
  binary_tree_array_list<T> &operator=(const binary_tree_array_list<T> &right) {
    deep_copy(right);
    return *this;
  }
}; // class binary_tree_array_list
} // namespace imdast

#endif // IMDAST_BINARY_TREE_ARRAY_LIST_H
