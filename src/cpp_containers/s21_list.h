#ifndef S21_LIST_H
#define S21_LIST_H

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>

namespace s21 {
template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  // constructors
  list() noexcept;
  list(size_type n);
  list(std::initializer_list<value_type> const& items) noexcept;
  list(const list& l) noexcept;
  list(list&& l) noexcept;
  list& operator=(const list& l);
  list& operator=(list&& l);
  ~list();

 private:
  struct Node {
    value_type data;
    Node* prev;
    Node* next;
    Node(const value_type val) : data(val), prev(nullptr), next(nullptr) {}
    Node() : data(), prev(nullptr), next(nullptr) {}
  };

  size_type size_;
  Node* tail;
  Node* head;
  Node* end_node;

 public:
  class ListConstIterator {
   protected:
    Node* cur_obj;

   public:
    ListConstIterator();
    ListConstIterator(Node* first);

    ListConstIterator& operator++();
    ListConstIterator operator++(int);
    ListConstIterator& operator--();
    ListConstIterator operator--(int);
    const_reference operator*();
    bool operator==(ListConstIterator other) const;
    bool operator!=(ListConstIterator other) const;
    Node* get_cur_obj() const;
  };

  class ListIterator : public ListConstIterator {
   public:
    ListIterator();
    ListIterator(Node* first);
    reference operator*() const;
  };

  using iterator = typename list<T>::ListIterator;
  using const_iterator = typename list<T>::ListConstIterator;
  // iterators
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  // accesors
  const_reference front() noexcept;
  const_reference back() noexcept;

  // List Capacity
  bool empty();
  size_type size();
  size_type max_size() const;

  // method
  void clear();
  iterator insert(iterator pos, const_reference value) noexcept;
  void erase(iterator pos);
  void push_back(const_reference value) noexcept;
  void push_front(const_reference value) noexcept;
  void pop_front();
  void pop_back();
  void reverse() noexcept;
  void swap(list& other) noexcept;
  void merge(list& other) noexcept;
  void splice(const_iterator pos, list& other);
  void unique() noexcept;
  void sort() noexcept;

  // other
  // void print_list();
};

// iter
template <typename T>
typename list<T>::const_iterator list<T>::begin() const {
  return const_iterator(head);
}

template <typename T>
typename list<T>::const_iterator list<T>::end() const {
  return const_iterator(end_node);
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
  return iterator(head);
}

template <typename T>
typename list<T>::iterator list<T>::end() {
  return iterator(end_node);
}

template <typename T>
list<T>::ListIterator::ListIterator() : ListConstIterator() {}

template <typename T>
list<T>::ListIterator::ListIterator(Node* first) : ListConstIterator(first) {}

template <typename T>
list<T>::ListConstIterator::ListConstIterator() {}

template <typename T>
list<T>::ListConstIterator::ListConstIterator(Node* first) : cur_obj(first) {}

// template <typename T>
// bool list<T>::ListIterator::operator<(const ListIterator& other) const {
//   if (cur_obj->data < other.cur_obj->data) {
//     return true;
//   } else {
//     return false;
//   }
// }

template <typename T>
typename list<T>::ListConstIterator& list<T>::ListConstIterator::operator++() {
  cur_obj = cur_obj->next;
  return *this;
}

template <typename T>
typename list<T>::ListConstIterator list<T>::ListConstIterator::operator++(
    int) {
  ++(*this);
  return *this;
}

template <typename T>
typename list<T>::Node* list<T>::ListConstIterator::get_cur_obj() const {
  return cur_obj;
}

template <typename T>
typename list<T>::ListConstIterator& list<T>::ListConstIterator::operator--() {
  cur_obj = cur_obj->prev;
  return *this;
}

template <typename T>
typename list<T>::ListConstIterator list<T>::ListConstIterator::operator--(
    int) {
  --(*this);
  return *this;
}

template <typename T>
bool list<T>::ListConstIterator::operator==(ListConstIterator other) const {
  return cur_obj == other.cur_obj;
}

template <typename T>
bool list<T>::ListConstIterator::operator!=(ListConstIterator other) const {
  return cur_obj != other.cur_obj;
}

template <typename T>
typename list<T>::reference list<T>::ListIterator::operator*() const {
  return this->cur_obj->data;
}

template <typename T>
typename list<T>::const_reference list<T>::ListConstIterator::operator*() {
  return cur_obj->data;
}

// constructors
template <typename T>
list<T>::list() noexcept {
  size_ = 0;
  end_node = new Node();
  end_node->next = end_node;
  end_node->prev = end_node;
  head = end_node;
  tail = end_node;
}

template <typename T>
list<T>::list(size_type n) : size_(0), tail(nullptr), head(nullptr) {
  if (n > max_size()) {
    throw std::invalid_argument("Invalid value");
  }
  end_node = new Node();
  end_node->prev = end_node;
  end_node->next = end_node;
  head = end_node;
  tail = end_node;
  for (size_type i = 0; i < n; ++i) {
    push_back(0);
  }
}

template <typename T>
list<T>::list(std::initializer_list<value_type> const& items) noexcept {
  size_ = 0;
  end_node = new Node();
  end_node->next = end_node;
  end_node->prev = end_node;
  head = end_node;
  tail = end_node;
  for (const const_reference item : items) {
    push_back(item);
  }
}

template <typename T>
list<T>::list(const list& l) noexcept {
  if (this != &l) {
    size_ = 0;
    end_node = new Node();
    end_node->next = end_node;
    end_node->prev = end_node;
    head = end_node;
    tail = end_node;
    Node* start = l.head;
    while (start != l.end_node) {
      push_back(start->data);
      start = start->next;
    }
  }
}

template <typename T>
list<T>::list(list&& l) noexcept {
  if (this != &l) {
    size_ = l.size_;
    head = l.head;
    tail = l.tail;
    end_node = l.end_node;

    l.size_ = 0;
    l.end_node = new Node();
    l.end_node->next = l.end_node;
    l.end_node->prev = l.end_node;
    l.tail = l.end_node;
    l.head = l.end_node;
    // l.clear();
  }
}

// copy
template <typename T>
typename list<T>::list& list<T>::operator=(const list& l) {
  if (this != &l) {
    clear();
    delete end_node;

    end_node = new Node();
    end_node->next = end_node;
    end_node->prev = end_node;
    head = end_node;
    tail = end_node;
    Node* start = l.head;
    while (start != l.end_node) {
      push_back(start->data);
      start = start->next;
    }
  }

  return *this;
}

template <typename T>
typename list<T>::list& list<T>::operator=(list&& l) {
  if (this != &l) {
    clear();
    delete end_node;

    size_ = l.size_;
    head = l.head;
    tail = l.tail;
    end_node = l.end_node;

    l.size_ = 0;
    l.end_node = new Node();
    l.end_node->next = l.end_node;
    l.end_node->prev = l.end_node;
    l.tail = l.end_node;
    l.head = l.end_node;
  }

  return *this;
}

template <typename T>
list<T>::~list() {
  clear();
  delete end_node;
}

// method
template <typename T>
void list<T>::push_back(const_reference value) noexcept {
  Node* temp = new Node(value);
  if (empty()) {
    tail = temp;
    head = temp;

    temp->prev = end_node;
    temp->next = end_node;

    end_node->prev = temp;
    end_node->next = temp;
  } else {
    temp->prev = tail;
    temp->next = end_node;

    tail->next = temp;
    end_node->prev = temp;
    tail = temp;
  }
  size_++;
}

template <typename T>
void list<T>::push_front(const_reference value) noexcept {
  Node* temp = new Node(value);
  if (empty()) {
    tail = temp;
    head = temp;

    temp->prev = end_node;
    temp->next = end_node;

    end_node->prev = temp;
    end_node->next = temp;
  } else {
    temp->prev = end_node;
    temp->next = head;

    head->prev = temp;
    end_node->next = temp;
    head = temp;
  }
  size_++;
}

template <typename T>
void list<T>::pop_back() {
  if (empty()) {
    throw std::length_error("list is empty");
  }
  if (size_ == 1) {
    delete tail;
    head = end_node;
    tail = end_node;
    end_node->next = end_node;
    end_node->prev = end_node;
  } else {
    Node* temp = tail;
    tail = temp->prev;
    tail->next = end_node;
    end_node->prev = tail;
    delete temp;
  }
  size_--;
}

template <typename T>
void list<T>::pop_front() {
  if (empty()) {
    throw std::length_error("list is empty");
  }
  if (size_ == 1) {
    delete head;
    head = end_node;
    tail = end_node;
    end_node->next = end_node;
    end_node->prev = end_node;
  } else {
    Node* temp = head;
    head = temp->next;
    head->prev = end_node;
    end_node->next = head;
    delete temp;
  }
  size_--;
}

template <typename T>
void list<T>::clear() {
  while (!empty()) {
    pop_front();
  }
  size_ = 0;
}

template <typename T>
void list<T>::erase(iterator pos) {
  if (empty() || pos == end()) {
    throw std::length_error("Cannot erase");
  }
  Node* tmp = pos.get_cur_obj();
  Node* prev_el = tmp->prev;
  Node* next_el = tmp->next;

  if (size_ == 1) {
    prev_el->next = next_el;
    next_el->prev = prev_el;
    head = end_node;
    tail = end_node;
  } else if (tmp == head) {
    head = next_el;
    head->prev = end_node;
    end_node->next = head;
  } else if (tmp == tail) {
    tail = prev_el;
    tail->next = end_node;
    end_node->prev = tail;
  } else {
    prev_el->next = next_el;
    next_el->prev = prev_el;
  }

  delete tmp;
  size_--;
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) noexcept {
  Node* right_node = pos.get_cur_obj();
  Node* left_node = right_node->prev;

  if (pos == tail) {
    push_back(value);
    return iterator(tail);
  }

  Node* new_node = new Node();
  if (right_node == head) {
    head = new_node;
    end_node->prev = new_node;
  }

  new_node->next = right_node;
  new_node->prev = left_node;
  new_node->data = value;
  right_node->prev = new_node;
  left_node->next = new_node;
  size_++;
  return iterator(new_node);
}

// accsesors
template <typename T>
typename list<T>::const_reference list<T>::front() noexcept {
  return head->data;
}

template <typename T>
typename list<T>::const_reference list<T>::back() noexcept {
  return tail->data;
}

// list capacity
template <typename T>
typename list<T>::size_type list<T>::size() {
  return size_;
}

template <typename T>
bool list<T>::empty() {
  return head == end_node && tail == end_node ? true : false;
}

template <typename T>
typename list<T>::size_type list<T>::max_size() const {
  return std::numeric_limits<size_type>::max() / sizeof(Node);
}

// reverse
template <typename T>
void list<T>::reverse() noexcept {
  if (size_ > 1) {
    Node* next_el = head;

    while (next_el != end_node) {
      std::swap(next_el->prev, next_el->next);
      next_el = next_el->prev;
    }

    std::swap(head, tail);
    end_node->next = head;
    end_node->prev = tail;
  }
}

// swap
template <typename T>
void list<T>::swap(list& other) noexcept {
  std::swap(end_node, other.end_node);
  std::swap(size_, other.size_);
  std::swap(head, other.head);
  std::swap(tail, other.tail);
}

template <typename T>
void list<T>::merge(list& other) noexcept {
  if (other.empty()) return;
  if (empty()) {
    swap(other);
    return;
  }

  s21::list<T> result;

  Node* first = head;
  Node* second = other.head;
  while (first != end_node && second != other.end_node) {
    if (first->data < second->data) {
      result.push_back(first->data);
      first = first->next;
    } else {
      result.push_back(second->data);
      second = second->next;
    }
  }

  if (first == end_node) {
    while (second != other.end_node) {
      result.push_back(second->data);
      second = second->next;
    }
  }

  if (second == other.end_node) {
    while (first != end_node) {
      result.push_back(first->data);
      first = first->next;
    }
  }

  swap(result);
  other.clear();
}

template <typename T>
void list<T>::splice(const_iterator pos, list& other) {
  Node* cur = pos.get_cur_obj();
  for (iterator it = other.begin(); it != other.end(); ++it) {
    insert(iterator(cur), *it);
  }
  other.clear();
}

template <typename T>
void list<T>::unique() noexcept {
  if (size_ > 1) {
    iterator cur = begin();
    iterator next = cur;
    ++next;
    while (next != end()) {
      if (*cur == *next) {
        erase(next);
        next = cur;
        ++next;
      } else {
        ++cur;
        ++next;
      }
    }
  }
}

template <typename T>
void list<T>::sort() noexcept {
  if (size_ > 1) {
    for (iterator it_1 = begin(); it_1 != end(); ++it_1) {
      iterator min_el = it_1;
      for (iterator it_2 = it_1; it_2 != end(); ++it_2) {
        if (*it_2 < *min_el) {
          min_el = it_2;
        }
      }
      std::swap(*it_1, *min_el);
    }
  }
}

}  // namespace s21

#endif