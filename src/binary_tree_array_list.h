#ifndef BINARY_TREE_ARRAY_LIST_H
#define BINARY_TREE_ARRAY_LIST_H

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <optional>
#include <stdexcept>

#define LEFT(n) ((n) * 2 + 1)
#define RIGHT(n) ((n) * 2 + 2)
#define PARENT(n) (((n) - 1) / 2)

template <class T> class binary_tree_array_list {
  std::optional<T> *_data;
  size_t _size;
  size_t _capacity;

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
        if (list->_data[offset + skip] == std::nullopt) {
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

    // Returns a pointer to the current item. May be nullptr.
    const T *operator*() const noexcept { return get(); }

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
      if (_list->_data[RIGHT(offset)] == std::nullopt ||
          RIGHT(offset) >= _list->_capacity) {
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
             _list->_data[LEFT(offset)] != std::nullopt) {
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
      if (_list->_data[LEFT(offset)] == std::nullopt ||
          LEFT(offset) >= _list->_capacity) {
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
             _list->_data[RIGHT(offset)] != std::nullopt) {
        offset = RIGHT(offset);
      }
      _current = &_list->_data[offset];
      return true;
    }
  };

  // Creates an empty binary tree array list.
  binary_tree_array_list() : _size(0), _capacity(0) {
    _data = static_cast<std::optional<T> *>(malloc(0));
  }

  ~binary_tree_array_list() noexcept { free(_data); }

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
    _data = malloc(_capacity * sizeof(std::optional<T>));
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
        for (size_t i = old_capacity; i < _capacity; i++) {
          _data[i] = std::nullopt;
        }
      }
      if (_data[index] == std::nullopt) {
        _data[index] = std::make_optional(value);
        _size++;
        return;
      }
      index = LEFT(index) + (_data[index].value() < value);
    }
  }

  // Returns an optional of the nth (0-indexed) item in the internal array of
  // the list. This does *not* return items in order. Only useful for debugging.
  std::optional<T> get_raw(size_t index) const noexcept {
    if (index >= _capacity)
      return std::nullopt;
    return _data[index];
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
};

#endif // BINARY_TREE_ARRAY_LIST_H
