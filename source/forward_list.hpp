#pragma once

#include <algorithm>
#include <stdexcept>
#include <utility>
namespace mystl {

template <typename T> class my_forward_list {
private:
  // node struct to hold data
  struct Node {
    T data;     // actual data stored in this node
    Node *next; // pointer to the next node in the list

    // constructor
    Node(const T &val, Node *nxt = nullptr) : data(val), next(nxt) {}

    // move constructor
    Node(T &&val, Node *nxt = nullptr) : data(std::move(val)), next(nxt) {}
  };
  // number of elements in the list
  size_t _size;

  // sentinel node before actual node to simplify operations
  Node *_before_head;

public:
  // default constructor and destructor
  my_forward_list() : _before_head(new Node(T())), _size(0) {
    _before_head->next = nullptr;
  }
  ~my_forward_list() {
    clear();
    delete _before_head;
  }

  // copy constructor
  my_forward_list(const my_forward_list &other)
      : _before_head(new Node(T())), _size(0) {
    _before_head->next = nullptr;

    Node *current = other._before_head->next;
    Node *tail = _before_head;

    while (current != nullptr) {
      tail->next = new Node(current->data);
      tail = tail->next;
      current = current->next;
      _size++;
    }
  }

  // copy operation
  my_forward_list &operator=(const my_forward_list &other) {
    // self assignment
    if (this == &other)
      return *this;

    clear();

    Node *current = other._before_head->next;
    Node *tail = _before_head;

    while (current != nullptr) {
      tail->next = new Node(current->data);
      tail = tail->next;
      current = current->next;
      _size++;
    }
    return *this;
  }

  // move constructor
  my_forward_list(my_forward_list &&other) noexcept
      : _before_head(other._before_head), _size(other._size) {
    // leave other in valid empty state
    other._before_head = new Node(t());
    other._before_head->next = nullptr;
    other._size = 0;
  }
  // move operation
  my_forward_list &operator=(my_forward_list &&other) noexcept {
    if (this != &other) {
      // clean up current resources
      clear();
      delete _before_head;

      _before_head = other._before_head;
      _size = other._size;

      // leave other in valid empty state
      other._before_head = new Node(T());
      other._before_head->next = nullptr;
      other._size = 0;
    }
    return *this;
  }

  // queries
  // size
  size_t size() const noexcept { return _size; }
  // empty
  bool empty() const noexcept { return _size == 0; }

  // basic operations
  // push front copy
  void push_front(const T &val) {
    // create new node
    Node *new_node = new Node(val, _before_head->next);
    // make sentinel node point to new node
    _before_head->next = new_node;
    _size++;
  }
  // push front move
  void push_front(T &&val) {
    // create new node by moving val
    Node *new_node = new Node(std::move(val), _before_head->next);
    // make sentinel node point to new node
    _before_head->next = new_node;
    _size++;
  }

  // pop front
  void pop_front() {
    // check if empty
    if (empty()) {
      throw std::out_of_range("Tried to use pop front on empty forward list.");
    }
    // save pointer to first node
    Node *old_front = _before_head->next;
    // make sentinel point to second node
    _before_head->next = old_front->next;
    // delete old first node
    delete old_front;
    _size--;
  }

  // front
  T &front() {
    // check if empty
    if (empty()) {
      throw std::out_of_range("Tried to access front on empty forward list.");
    }
    return _before_head->next->data;
  }
  T &front() const {
    // check if empty
    if (empty()) {
      throw std::out_of_range("Tried to access front on empty forward list.");
    }
    return _before_head->next->data;
  }
  // clear, remove all elements but keep sentinel
  void clear() {
    Node *current = _before_head->next;
    while (current != nullptr) {
      Node *next = current->next;
      delete current;
      current = next;
    }
    _before_head->next = nullptr;
    _size = 0;
  }

  // custom iterator classes
  class iterator {
  private:
    Node *_node; // pointer to the current node we're at

    // only my_forward_list can create iterators with specific nodes
    friend class my_forward_list;
    iterator(Node *n) : _node(n) {}

  public:
    // iterator trait definitions (needed for STL compatibility)
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    // dereference - access the data in current node
    T &operator*() const { return _node->data; }

    // arrow operator - access members of data in current node
    T *operator->() const { return &(_node->data); }

    // pre-increment - move to next node, return reference to this iterator
    iterator &operator++() {
      _node = _node->next;
      return *this;
    }

    // post-increment - move to next node, return copy of old position
    iterator operator++(int) {
      iterator tmp = *this;
      _node = _node->next;
      return tmp;
    }

    // equality comparison - check if two iterators point to same node
    bool operator==(const iterator &other) const {
      return _node == other._node;
    }

    // inequality comparison - check if two iterators point to different nodes
    bool operator!=(const iterator &other) const {
      return _node != other._node;
    }
  };
  class const_iterator {
  private:
    const Node *_node; // pointer to current node (const to prevent
                       // modification)

    friend class my_forward_list;
    const_iterator(const Node *n) : _node(n) {}

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    const T &operator*() const { return _node->data; }
    const T *operator->() const { return &(_node->data); }

    const_iterator &operator++() {
      _node = _node->next;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp = *this;
      _node = _node->next;
      return tmp;
    }

    bool operator==(const const_iterator &other) const {
      return _node == other._node;
    }
    bool operator!=(const const_iterator &other) const {
      return _node != other._node;
    }
  };
  // iterator implementation

  iterator before_begin() { return iterator(_before_head); }
  const_iterator before_begin() const { return const_iterator(_before_head); }
  const_iterator cbefore_begin() const { return const_iterator(_before_head); }

  iterator begin() { return iterator(_before_head->next); }
  const_iterator begin() const { return const_iterator(_before_head->next); }
  const_iterator cbegin() const { return const_iterator(_before_head->next); }

  iterator end() { return iterator(nullptr); }
  const_iterator end() const { return const_iterator(nullptr); }
  const_iterator cend() const { return const_iterator(nullptr); }

  // iterator operations
  iterator insert_after(iterator pos, const T &val) {
    // create new node pointing to the node after pos
    Node *new_node = new Node(val, pos._node->next);
    // make pos point to new node
    pos._node->next = new_node;
    _size++;
    // return iterator to newly inserted element
    return iterator(new_node);
  }

  // insert_after, move
  iterator insert_after(iterator pos, T &&val) {
    // create new node by moving val, pointing to node after pos
    Node *new_node = new Node(std::move(val), pos._node->next);
    // make pos point to new node
    pos._node->next = new_node;
    _size++;
    // return iterator to newly inserted element
    return iterator(new_node);
  }

  // erase_after
  iterator erase_after(iterator pos) {
    if (pos._node->next == nullptr) {
      throw std::out_of_range("Tried to erase after at end of forward list.");
    }

    // save pointer to node we're removing
    Node *to_delete = pos._node->next;
    // make pos skip over the deleted node
    pos._node->next = to_delete->next;
    // save iterator to the node after deleted one
    iterator result(pos._node->next);
    // delete the node
    delete to_delete;
    _size--;
    // return iterator to element after deleted one
    return result;
  }

  // equality operator
  bool operator==(const my_forward_list &other) const {
    // different sizes means not equal
    if (_size != other._size)
      return false;

    // compare element by element
    Node *current1 = _before_head->next;
    Node *current2 = other._before_head->next;

    while (current1 != nullptr) {
      if (current1->data != current2->data)
        return false;
      current1 = current1->next;
      current2 = current2->next;
    }

    return true;
  }

  // inequality operator
  bool operator!=(const my_forward_list &other) const {
    return !(*this == other);
  }
};

} // namespace mystl