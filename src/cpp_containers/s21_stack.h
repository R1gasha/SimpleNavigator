#ifndef S21_STACK_H
#define S21_STACK_H

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>

#include "s21_list.h"

namespace s21 {
template <typename T, typename Container = s21::list<T>>
class stack {
 private:
  Container container_list;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

 public:
  stack() : container_list(){};
  stack(std::initializer_list<value_type> const &items)
      : container_list(items){};
  stack(const stack &s) : container_list(s.container_list){};
  stack(stack &&s) : container_list(std::move(s.container_list)){};
  ~stack(){};
  stack &operator=(const stack &s);
  stack &operator=(stack &&s);

  // geter
  const_reference top();

  // capacity
  bool empty();
  size_type size();

  // method
  void push(const_reference value);
  void pop();
  void swap(stack &other);
};

template <typename T, typename Container>
typename stack<T, Container>::const_reference stack<T, Container>::top() {
  if (container_list.empty()) throw std::out_of_range("Out of range");
  return container_list.back();
}

template <typename T, typename Container>
bool stack<T, Container>::empty() {
  return container_list.empty();
}

template <typename T, typename Container>
typename stack<T, Container>::size_type stack<T, Container>::size() {
  return container_list.size();
}

template <typename T, typename Container>
void stack<T, Container>::push(const_reference value) {
  container_list.push_back(value);
}

template <typename T, typename Container>
void stack<T, Container>::pop() {
  if (container_list.empty()) {
    throw std::length_error("Stack is empty");
  }
  container_list.pop_back();
}

template <typename T, typename Container>
void stack<T, Container>::swap(stack &other) {
  container_list.swap(other.container_list);
}

// move
template <typename T, typename Container>
typename stack<T, Container>::stack &stack<T, Container>::operator=(stack &&s) {
  this->container_list.clear();
  this->container_list = std::move(s.container_list);
  return *this;
}

// copy
template <typename T, typename Container>
typename stack<T, Container>::stack &stack<T, Container>::operator=(
    const stack &s) {
  this->container_list.clear();
  this->container_list = s.container_list;
  return *this;
}

}  // namespace s21

#endif
