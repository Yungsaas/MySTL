#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace mystl {
template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class my_unordered_map {
private:
  // node struct to hold key-value pairs in chains
  struct Node {
    std::pair<const Key, Value> data; // key-value pair
    Node *next;                       // pointer to next node in chain

    // constructor
    Node(const Key &k, const Value &v, Node *nxt = nullptr)
        : data(k, v), next(nxt) {}

    // move constructor
    Node(Key &&k, Value &&v, Node *nxt = nullptr)
        : data(std::move(k), std::move(v)), next(nxt) {}
  };

  // vector of bucket heads
  std::vector<Node *> _buckets;
  // number of elements in the map
  size_t _size;
  // hasher and equality comparator
  Hash _hasher;
  KeyEqual _key_equal;
  // max load factor before rehash
  float _max_load_factor;

  // helper to get bucket index for a key
  size_t get_bucket_index(const Key &key) const {
    return _hasher(key) % _buckets.size();
  }

  // helper to find node with given key in a bucket
  Node *find_in_bucket(size_t bucket_idx, const Key &key) const {
    Node *current = _buckets[bucket_idx];
    while (current != nullptr) {
      if (_key_equal(current->data.first, key)) {
        return current;
      }
      current = current->next;
    }
    return nullptr;
  }

public:
  // default constructor
  my_unordered_map(size_t bucket_count = 8, const Hash &hash = Hash(),
                   const KeyEqual &equal = KeyEqual())
      : _buckets(bucket_count, nullptr), _size(0), _hasher(hash),
        _key_equal(equal), _max_load_factor(1.0f) {}

  // destructor
  ~my_unordered_map() { clear(); }

  // copy constructor
  my_unordered_map(const my_unordered_map &other)
      : _buckets(other._buckets.size(), nullptr), _size(0),
        _hasher(other._hasher), _key_equal(other._key_equal),
        _max_load_factor(other._max_load_factor) {
    // copy all elements
    for (size_t i = 0; i < other._buckets.size(); i++) {
      Node *current = other._buckets[i];
      while (current != nullptr) {
        insert(current->data.first, current->data.second);
        current = current->next;
      }
    }
  }

  // copy assignment
  my_unordered_map &operator=(const my_unordered_map &other) {
    // self assignment check
    if (this == &other)
      return *this;

    clear();
    _buckets.resize(other._buckets.size(), nullptr);
    _hasher = other._hasher;
    _key_equal = other._key_equal;
    _max_load_factor = other._max_load_factor;

    // copy all elements
    for (size_t i = 0; i < other._buckets.size(); i++) {
      Node *current = other._buckets[i];
      while (current != nullptr) {
        insert(current->data.first, current->data.second);
        current = current->next;
      }
    }
    return *this;
  }

  // move constructor
  my_unordered_map(my_unordered_map &&other) noexcept
      : _buckets(std::move(other._buckets)), _size(other._size),
        _hasher(std::move(other._hasher)),
        _key_equal(std::move(other._key_equal)),
        _max_load_factor(other._max_load_factor) {
    // leave other in valid empty state
    other._size = 0;
  }

  // move assignment
  my_unordered_map &operator=(my_unordered_map &&other) noexcept {
    if (this != &other) {
      // clean up current resources
      clear();

      // take ownership of other's data
      _buckets = std::move(other._buckets);
      _size = other._size;
      _hasher = std::move(other._hasher);
      _key_equal = std::move(other._key_equal);
      _max_load_factor = other._max_load_factor;

      // leave other in valid empty state
      other._size = 0;
    }
    return *this;
  }

  // queries
  size_t size() const noexcept { return _size; }
  bool empty() const noexcept { return _size == 0; }
  size_t bucket_count() const noexcept { return _buckets.size(); }

  // load factor
  float load_factor() const noexcept {
    return _buckets.size() == 0 ? 0.0f
                                : static_cast<float>(_size) / _buckets.size();
  }

  float max_load_factor() const noexcept { return _max_load_factor; }

  void max_load_factor(float ml) {
    if (ml <= 0.0f) {
      throw std::invalid_argument("max load factor must be positive");
    }
    _max_load_factor = ml;
  }

  // rehash to at least n buckets
  void rehash(size_t n) {
    if (n <= _buckets.size())
      return;

    // save old buckets
    std::vector<Node *> old_buckets = std::move(_buckets);
    // create new buckets
    _buckets = std::vector<Node *>(n, nullptr);
    _size = 0;

    // reinsert all elements
    for (size_t i = 0; i < old_buckets.size(); i++) {
      Node *current = old_buckets[i];
      while (current != nullptr) {
        Node *next = current->next;
        // reinsert using existing node
        size_t new_idx = get_bucket_index(current->data.first);
        current->next = _buckets[new_idx];
        _buckets[new_idx] = current;
        _size++;
        current = next;
      }
    }
  }

  // insert copy
  std::pair<bool, Value *> insert(const Key &key, const Value &val) {
    // check if rehash needed
    if (load_factor() >= _max_load_factor) {
      rehash(_buckets.size() * 2);
    }

    size_t idx = get_bucket_index(key);

    // check if key already exists
    Node *existing = find_in_bucket(idx, key);
    if (existing != nullptr) {
      return {false, &existing->data.second};
    }

    // create new node at front of bucket
    Node *new_node = new Node(key, val, _buckets[idx]);
    _buckets[idx] = new_node;
    _size++;
    return {true, &new_node->data.second};
  }

  // insert move
  std::pair<bool, Value *> insert(Key &&key, Value &&val) {
    // check if rehash needed
    if (load_factor() >= _max_load_factor) {
      rehash(_buckets.size() * 2);
    }

    size_t idx = get_bucket_index(key);

    // check if key already exists
    Node *existing = find_in_bucket(idx, key);
    if (existing != nullptr) {
      return {false, &existing->data.second};
    }

    // create new node at front of bucket
    Node *new_node = new Node(std::move(key), std::move(val), _buckets[idx]);
    _buckets[idx] = new_node;
    _size++;
    return {true, &new_node->data.second};
  }

  // find
  Value *find(const Key &key) {
    size_t idx = get_bucket_index(key);
    Node *node = find_in_bucket(idx, key);
    return node != nullptr ? &node->data.second : nullptr;
  }

  const Value *find(const Key &key) const {
    size_t idx = get_bucket_index(key);
    Node *node = find_in_bucket(idx, key);
    return node != nullptr ? &node->data.second : nullptr;
  }

  // erase by key
  bool erase(const Key &key) {
    size_t idx = get_bucket_index(key);

    Node *current = _buckets[idx];
    Node *prev = nullptr;

    while (current != nullptr) {
      if (_key_equal(current->data.first, key)) {
        // found node to delete
        if (prev == nullptr) {
          // node is at front of bucket
          _buckets[idx] = current->next;
        } else {
          // node is in middle or end
          prev->next = current->next;
        }
        delete current;
        _size--;
        return true;
      }
      prev = current;
      current = current->next;
    }
    return false;
  }

  // clear all elements
  void clear() {
    for (size_t i = 0; i < _buckets.size(); i++) {
      Node *current = _buckets[i];
      while (current != nullptr) {
        Node *next = current->next;
        delete current;
        current = next;
      }
      _buckets[i] = nullptr;
    }
    _size = 0;
  }

  // operator[] for access/insert
  Value &operator[](const Key &key) {
    size_t idx = get_bucket_index(key);
    Node *node = find_in_bucket(idx, key);

    if (node != nullptr) {
      return node->data.second;
    }

    // key doesn't exist, insert with default value
    auto result = insert(key, Value());
    return *result.second;
  }

  // at for access with bounds checking
  Value &at(const Key &key) {
    Value *val = find(key);
    if (val == nullptr) {
      throw std::out_of_range("key not found in unordered_map");
    }
    return *val;
  }

  const Value &at(const Key &key) const {
    const Value *val = find(key);
    if (val == nullptr) {
      throw std::out_of_range("key not found in unordered_map");
    }
    return *val;
  }

  // check if key exists
  bool contains(const Key &key) const { return find(key) != nullptr; }

  // custom iterator class
  class iterator {
  private:
    my_unordered_map *_map; // pointer to the map
    size_t _bucket_idx;     // current bucket index
    Node *_node;            // current node in bucket

    friend class my_unordered_map;

    iterator(my_unordered_map *map, size_t bucket, Node *node)
        : _map(map), _bucket_idx(bucket), _node(node) {
      // if node is null, advance to next valid node
      if (_node == nullptr) {
        advance_to_next();
      }
    }

    void advance_to_next() {
      // already at end
      if (_bucket_idx >= _map->_buckets.size())
        return;

      // try next node in current bucket
      if (_node != nullptr) {
        _node = _node->next;
      }

      // if no node, find next non-empty bucket
      while (_node == nullptr && _bucket_idx < _map->_buckets.size()) {
        _bucket_idx++;
        if (_bucket_idx < _map->_buckets.size()) {
          _node = _map->_buckets[_bucket_idx];
        }
      }
    }

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<const Key, Value>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    // dereference
    std::pair<const Key, Value> &operator*() const { return _node->data; }

    // arrow operator
    std::pair<const Key, Value> *operator->() const { return &_node->data; }

    // pre-increment
    iterator &operator++() {
      advance_to_next();
      return *this;
    }

    // post-increment
    iterator operator++(int) {
      iterator tmp = *this;
      advance_to_next();
      return tmp;
    }

    // equality comparison
    bool operator==(const iterator &other) const {
      return _map == other._map && _bucket_idx == other._bucket_idx &&
             _node == other._node;
    }

    // inequality comparison
    bool operator!=(const iterator &other) const { return !(*this == other); }
  };

  class const_iterator {
  private:
    const my_unordered_map *_map;
    size_t _bucket_idx;
    const Node *_node;

    friend class my_unordered_map;

    const_iterator(const my_unordered_map *map, size_t bucket, const Node *node)
        : _map(map), _bucket_idx(bucket), _node(node) {
      if (_node == nullptr) {
        advance_to_next();
      }
    }

    void advance_to_next() {
      if (_bucket_idx >= _map->_buckets.size())
        return;

      if (_node != nullptr) {
        _node = _node->next;
      }

      while (_node == nullptr && _bucket_idx < _map->_buckets.size()) {
        _bucket_idx++;
        if (_bucket_idx < _map->_buckets.size()) {
          _node = _map->_buckets[_bucket_idx];
        }
      }
    }

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<const Key, Value>;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;

    const std::pair<const Key, Value> &operator*() const { return _node->data; }
    const std::pair<const Key, Value> *operator->() const {
      return &_node->data;
    }

    const_iterator &operator++() {
      advance_to_next();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp = *this;
      advance_to_next();
      return tmp;
    }

    bool operator==(const const_iterator &other) const {
      return _map == other._map && _bucket_idx == other._bucket_idx &&
             _node == other._node;
    }

    bool operator!=(const const_iterator &other) const {
      return !(*this == other);
    }
  };

  // iterator methods
  iterator begin() {
    for (size_t i = 0; i < _buckets.size(); i++) {
      if (_buckets[i] != nullptr) {
        return iterator(this, i, _buckets[i]);
      }
    }
    return end();
  }

  iterator end() { return iterator(this, _buckets.size(), nullptr); }

  const_iterator begin() const {
    for (size_t i = 0; i < _buckets.size(); i++) {
      if (_buckets[i] != nullptr) {
        return const_iterator(this, i, _buckets[i]);
      }
    }
    return end();
  }

  const_iterator end() const {
    return const_iterator(this, _buckets.size(), nullptr);
  }

  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  // equality operator
  bool operator==(const my_unordered_map &other) const {
    // different sizes means not equal
    if (_size != other._size)
      return false;

    // check that every key-value pair in this map exists in other
    for (size_t i = 0; i < _buckets.size(); i++) {
      Node *current = _buckets[i];
      while (current != nullptr) {
        const Value *other_val = other.find(current->data.first);
        if (other_val == nullptr || *other_val != current->data.second) {
          return false;
        }
        current = current->next;
      }
    }
    return true;
  }

  // inequality operator
  bool operator!=(const my_unordered_map &other) const {
    return !(*this == other);
  }
};
} // namespace mystl