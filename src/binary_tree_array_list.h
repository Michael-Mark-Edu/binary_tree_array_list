#ifndef BINARY_TREE_ARRAY_LIST_H
#define BINARY_TREE_ARRAY_LIST_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <utility>

#define LEFT(n) ((n) * 2 + 1)
#define RIGHT(n) ((n) * 2 + 2)
#define PARENT(n) (((n) - 1) / 2)
#define IS_LEFT(n) ((n) % 2 == 1)
#define IS_RIGHT(n) ((n) % 2 == 0)

template <class T> class binary_tree_array_list {
  std::optional<T> *_data;
  // 256 layers should be more than enough layers for a balanced AVL tree that
  // can be indexed by up to 2^64. Thanks to AVL tree invariants, the maximum
  // number of layers needed should only be a few more than 64.
  uint8_t *_height;
  size_t _size;
  size_t _capacity;

  void swim(size_t current, size_t skip) {
    if (current >= _capacity || !_data[current].has_value())
      return;
    _data[current - skip] = std::move(_data[current]);
    _height[current - skip] = std::move(_height[current]);
    _data[current].reset();
    _height[current] = 0;
    swim(LEFT(current), skip * 2);
    swim(RIGHT(current), skip * 2);
  }

  void sink(size_t current, size_t skip) {
    if (current >= _capacity || !_data[current].has_value())
      return;
    if (current + skip >= _capacity) {
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

    sink(LEFT(current), skip * 2);
    sink(RIGHT(current), skip * 2);

    _data[current + skip] = std::move(_data[current]);
    _height[current + skip] = std::move(_height[current]);
    _data[current].reset();
    _height[current] = 0;
  }

  void shift(size_t current, long long shift_amount) {
    if (current >= _capacity || !_data[current].has_value() ||
        shift_amount == 0)
      return;
    _data[current + shift_amount] = std::move(_data[current]);
    _height[current + shift_amount] = std::move(_height[current]);
    _data[current].reset();
    _height[current] = 0;

    shift(LEFT(current), shift_amount * 2);
    shift(RIGHT(current), shift_amount * 2);
  }

public:
  class iterator {
    const binary_tree_array_list<T> *_list;
    // Invariant: cannot be nullopt (but it can be nullptr)
    std::optional<T> *_current;
    size_t _index;

    inline void
    construct_at_zero(const binary_tree_array_list<T> *list) noexcept {
      if (list->_size == 0) {
        _current = nullptr;
        return;
      }
      size_t offset = 0;
      size_t skip = 1;
      while (list->_capacity >= offset + skip) {
        if (!list->_data[offset + skip].has_value()) {
          _current = &list->_data[offset];
          return;
        }
        offset += skip;
        skip *= 2;
      }
      _current = &list->_data[offset];
    }

  public:
    // Creates an iterator pointing to the smallest item in the list.
    iterator(const binary_tree_array_list<T> *list) noexcept
        : _list(list), _index(0) {
      construct_at_zero(list);
    }

    // Creates an iterator pointing to the nth (0-indexed) smallest item in the
    // list. If index is >= list->size(), then the iterator will point to the
    // greatest item in the list.
    iterator(const binary_tree_array_list<T> *list, size_t index) noexcept
        : _list(list), _index(0) {
      construct_at_zero(list);
      for (size_t i = 0; i < index; i++) {
        this->next();
      }
      _index = std::min(index, _index);
    }

    // Returns a pointer to the current item. May be nullptr.
    const T *get() const noexcept {
      if (_current == nullptr)
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
      if (_current == nullptr)
        return false;
      _index++;
      size_t offset = (_current - _list->_data) * 8 / sizeof(std::optional<T>);
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
      if (_index == 0)
        return false;
      _index--;
      size_t offset = (_current - _list->_data) * 8 / sizeof(std::optional<T>);
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

    // Returns the value at the iterator's current position. Throws an exception
    // if called on the past-the-last item. If this behavior is not desired,
    // then use the get() method instead.
    T operator*() const { return *get(); }

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
  };

  // Creates an empty binary tree array list.
  binary_tree_array_list()
      : _data(nullptr), _height(nullptr), _size(0), _capacity(0) {}

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
          sink(RIGHT(x), RIGHT(RIGHT(x)) - RIGHT(x));
          _data[RIGHT(x)] = std::move(_data[y]);
          shift(RIGHT(y), 1);
          swim(z, z - y);

          _height[LEFT(x)] =
              std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
          _height[RIGHT(x)] =
              std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
          _height[x] = std::max(_height[LEFT(x)], _height[RIGHT(x)]) + 1;

          break;

        // Rotate right-left
        case 1:
          sink(LEFT(x), LEFT(LEFT(x)) - LEFT(x));
          _data[LEFT(x)] = std::move(_data[x]);
          _data[x] = std::move(_data[z]);
          _data[z].reset();
          _height[z] = 0;
          shift(LEFT(z), RIGHT(LEFT(x)) - LEFT(z));
          swim(RIGHT(z), RIGHT(z) - z);

          _height[LEFT(x)] =
              std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
          _height[RIGHT(x)] =
              std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
          _height[x] = std::max(_height[LEFT(x)], _height[RIGHT(x)]) + 1;

          break;

        // Rotate left-right
        case 2:
          sink(RIGHT(x), RIGHT(RIGHT(x)) - RIGHT(x));
          _data[RIGHT(x)] = std::move(_data[x]);
          _data[x] = std::move(_data[z]);
          _data[z].reset();
          _height[z] = 0;
          shift(RIGHT(z), LEFT(RIGHT(x)) - RIGHT(z));
          swim(LEFT(z), LEFT(z) - z);

          _height[LEFT(x)] =
              std::max(_height[LEFT(LEFT(x))], _height[RIGHT(LEFT(x))]) + 1;
          _height[RIGHT(x)] =
              std::max(_height[LEFT(RIGHT(x))], _height[RIGHT(RIGHT(x))]) + 1;
          _height[x] = std::max(_height[LEFT(x)], _height[RIGHT(x)]) + 1;

          break;

        // Rotate left
        case 3:
          std::swap(_data[x], _data[y]);
          sink(LEFT(x), LEFT(LEFT(x)) - LEFT(x));
          _data[LEFT(x)] = std::move(_data[y]);
          shift(LEFT(y), -1);
          swim(z, z - y);

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
};

#endif // BINARY_TREE_ARRAY_LIST_H
