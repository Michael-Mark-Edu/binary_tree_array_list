#ifndef BINARY_TREE_ARRAY_LIST_H
#define BINARY_TREE_ARRAY_LIST_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <utility>

#define LEFT(n) ((n) * 2 + 1)
#define RIGHT(n) ((n) * 2 + 2)
#define PARENT(n) (((n) - 1) / 2)
#define IS_LEFT(n) ((n) % 2 == 1)
#define IS_RIGHT(n) ((n) % 2 == 0)

namespace imdast {
template <class T> class binary_tree_array_list {
  std::optional<T> *_data;
  // 256 layers should be more than enough layers for a balanced AVL tree that
  // can be indexed by up to 2^64. Thanks to AVL tree invariants, the maximum
  // number of layers needed should only be a few more than 64.
  uint8_t *_height;
  size_t _size;
  size_t _capacity;

  void shift(size_t current, long long shift_amount) {
    if (current >= _capacity || !_data[current].has_value() ||
        shift_amount == 0)
      return;

    while (current + shift_amount >= _capacity) {
      size_t old_capacity = _capacity;
      _capacity = LEFT(_capacity);
      _data = static_cast<std::optional<T> *>(
          realloc(_data, _capacity * sizeof(std::optional<T>)));
      _height =
          static_cast<uint8_t *>(realloc(_height, _capacity * sizeof(uint8_t)));
      for (size_t i = old_capacity; i < _capacity; i++) {
        _data[i] = std::optional<T>();
        _height[i] = 0;
      }
    }

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

public:
  class iterator {
    const binary_tree_array_list<T> *_list;
    // Invariant: cannot be nullopt (but it can be nullptr)
    std::optional<T> *_current;
    size_t _index;

    void construct_at_zero() noexcept {
      if (_list->_size == 0) {
        _current = nullptr;
        return;
      }
      size_t offset = 0;
      while (LEFT(offset) < _list->_capacity) {
        if (!_list->_data[LEFT(offset)].has_value()) {
          _current = &_list->_data[offset];
          return;
        }
        offset = LEFT(offset);
      }
      _current = &_list->_data[offset];
    }

  public:
    // Creates an iterator with no associated list.
    iterator() noexcept : _list(nullptr), _current(nullptr), _index(0) {}

    // Creates an iterator pointing to the smallest item in the list.
    iterator(const binary_tree_array_list<T> *list) noexcept
        : _list(list), _index(0) {
      construct_at_zero();
    }

    // Creates an iterator pointing to the nth (0-indexed) smallest item in the
    // list. If index is >= list->size(), then the iterator will point to the
    // greatest item in the list.
    iterator(const binary_tree_array_list<T> *list, size_t index) noexcept
        : _list(list), _index(0) {
      construct_at_zero();
      for (size_t i = 0; i < index; i++) {
        this->next();
      }
      _index = std::min(index, _index);
    }

    // Performs a shallow copy of the iterator. The copy will act independently
    // from the original iterator.
    iterator(const binary_tree_array_list<T>::iterator &iter) noexcept
        : _list(iter._list), _current(iter._current), _index(iter._index) {}

    // Returns a pointer to the current item. May be nullptr.
    const T *get() const noexcept {
      if (!_list || !_current)
        return nullptr;
      else
        return &_current->value();
    }

    // Returns the index of the iterator.
    size_t index() const noexcept { return _index; }

    // Checks if calling next() would pass or fail.
    bool has_next() const noexcept { return _current != nullptr; }

    // Checks if calling prev() would pass or fail.
    bool has_prev() const noexcept { return _index > 0; }

    // Moves the iterator to the next item in the list. Returns whether the
    // iterator actually moved. Final item is nullptr.
    bool next() noexcept {
      if (!_list || !_current)
        return false;
      _index++;
      size_t offset = _current - _list->_data;
      if (RIGHT(offset) >= _list->_capacity ||
          !_list->_data[RIGHT(offset)].has_value()) {
        T value = _current[0].value();
        size_t parent = offset;
        do {
          if (parent == 0) {
            _current = nullptr;
            return true;
          }
          parent = PARENT(parent);
        } while (_list->_data[parent].value() < value);
        _current = &_list->_data[parent];
        return true;
      }
      offset = RIGHT(offset);
      while (LEFT(offset) < _list->_capacity &&
             _list->_data[LEFT(offset)].has_value()) {
        offset = LEFT(offset);
      }
      _current = &_list->_data[offset];
      return true;
    }

    // Moves the iterator to the previous item in the list. Returns whether the
    // iterator actually moved. Final item is the first item.
    bool prev() noexcept {
      if (!_list || _index == 0 || _list->_size == 0)
        return false;
      _index--;
      if (!_current) {
        size_t index = 0;
        while (RIGHT(index) < _list->_capacity &&
               _list->_data[RIGHT(index)].has_value()) {
          index = RIGHT(index);
        }
        _current = &_list->_data[index];
        return true;
      }
      size_t offset = _current - _list->_data;
      if (LEFT(offset) >= _list->_capacity ||
          !_list->_data[LEFT(offset)].has_value()) {
        T value = _current[0].value();
        size_t parent = offset;
        do {
          if (parent == 0) {
            _current = nullptr;
            return true;
          }
          parent = PARENT(parent);
        } while (_list->_data[parent].value() > value);
        _current = &_list->_data[parent];
        return true;
      }
      offset = LEFT(offset);
      while (RIGHT(offset) < _list->_capacity &&
             _list->_data[RIGHT(offset)].has_value()) {
        offset = RIGHT(offset);
      }
      _current = &_list->_data[offset];
      return true;
    }

    // Returns the value at the iterator's current position. Throws a
    // std::logic_error if called on the past-the-last item. If this behavior is
    // not desired, then use the get() method instead.
    T operator*() const {
      const T *item = get();
      return item ? *item
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
      this->_index = right._index;
      return *this;
    }
  };

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
    _height = static_cast<uint8_t *>(malloc(_capacity * sizeof(uint8_t)));
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
      _height[PARENT(index)] = std::max(
          _height[PARENT(index)], static_cast<uint8_t>(_height[index] + 1));
      index = PARENT(index);
      size_t x = index;

      if (std::abs(_height[RIGHT(x)] - _height[LEFT(x)]) >= 2) {
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

          _height[LEFT(x)] =
              std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
          _height[RIGHT(x)] =
              std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
          _height[x] = std::max(_height[LEFT(x)], _height[RIGHT(x)]) + 1;

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

          _height[LEFT(x)] =
              std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
          _height[RIGHT(x)] =
              std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
          _height[x] = std::max(_height[LEFT(x)], _height[RIGHT(x)]) + 1;

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

          _height[LEFT(x)] =
              std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
          _height[RIGHT(x)] =
              std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
          _height[x] = std::max(_height[LEFT(x)], _height[RIGHT(x)]) + 1;

          break;

        // Rotate left
        case 3:
          std::swap(_data[x], _data[y]);
          shift(LEFT(x), LEFT(LEFT(x)) - LEFT(x));
          _data[LEFT(x)] = std::move(_data[y]);
          shift(LEFT(y), -1);
          shift(z, y - z);

          _height[LEFT(x)] =
              std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
          _height[RIGHT(x)] =
              std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
          _height[x] = std::max(_height[RIGHT(x)], _height[LEFT(x)]) + 1;

          break;

        default:
          std::unreachable();
        }
      }
    }
  }

  // Returns a const pointer to the nth (0-indexed) item in the list. Unlike the
  // [] operator, this method cannot throw an exception if index is too large.
  const T *get(size_t index) const noexcept {
    iterator iter = iterator(this);
    for (size_t i = 0; i < index; i++) {
      if (!iter.next())
        return nullptr;
    }
    return iter.get();
  }

  // Returns a const reference to the nth (0-indexed) item in the list. If index
  // is >= size(), then a std::logic_error is thrown. If this behavior is not
  // desired, then use the get() method instead.
  const T &operator[](size_t index) const {
    if (index >= _size)
      throw std::logic_error("Subscript out-of-bounds");
    iterator iter = iterator(this);
    for (size_t i = 0; i < index; i++) {
      iter.next();
    }
    return *iter.get();
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
};
} // namespace imdast

#endif // BINARY_TREE_ARRAY_LIST_H
