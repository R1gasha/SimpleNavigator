#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>

#include "s21_list.h"

namespace s21 {
template <typename T, typename Container = s21::list<T>>
class queue {
 private:
  Container container_list;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;

 public:
  queue() : container_list(){};
  queue(std::initializer_list<value_type> const &items)
      : container_list(items){};
  queue(const queue &q) : container_list(q.container_list){};
  queue(queue &&q) : container_list(std::move(q.container_list)){};
  ~queue(){};
  queue &operator=(const queue &q);
  queue &operator=(queue &&q);

 public:
  // accsessros
  const_reference front();
  const_reference back();

  // capacity
  bool empty();
  size_type size();

  // method
  void push(const_reference value);
  void pop();
  void swap(queue &other);
};

template <typename T, typename Container>
typename queue<T, Container>::const_reference queue<T, Container>::front() {
  if (empty()) throw std::length_error("Queue is empty");
  return container_list.front();
}

template <typename T, typename Container>
typename queue<T, Container>::const_reference queue<T, Container>::back() {
  if (empty()) throw std::length_error("Queue is empty");
  return container_list.back();
}

template <typename T, typename Container>
void queue<T, Container>::push(const_reference value) {
  container_list.push_back(value);
}

template <typename T, typename Container>
void queue<T, Container>::pop() {
  if (empty()) throw std::length_error("Queue is empty");
  container_list.pop_front();
}

template <typename T, typename Container>
void queue<T, Container>::swap(queue &other) {
  container_list.swap(other.container_list);
}

template <typename T, typename Container>
bool queue<T, Container>::empty() {
  return container_list.empty();
}

template <typename T, typename Container>
typename queue<T, Container>::size_type queue<T, Container>::size() {
  return container_list.size();
}

// move
template <typename T, typename Container>
typename queue<T, Container>::queue &queue<T, Container>::operator=(queue &&q) {
  this->container_list.clear();
  this->container_list = std::move(q.container_list);
  return *this;
}

// copy
template <typename T, typename Container>
typename queue<T, Container>::queue &queue<T, Container>::operator=(
    const queue &q) {
  this->container_list.clear();
  this->container_list = q.container_list;
  return *this;
}

}  // namespace s21

#endif
