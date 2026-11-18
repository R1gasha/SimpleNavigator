#ifndef S21_SET_H
#define S21_SET_H
#include <initializer_list>
#include <limits>

#include "s21_rbtree.h"

namespace s21 {
template <typename T>
class set {
 private:
  s21::RBTree<T, T> tree_;

 public:
  using key_type = T;
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename s21::RBTree<T, T>::const_iterator;
  using const_iterator = typename s21::RBTree<T, T>::const_iterator;
  using size_type = std::size_t;

  set() = default;
  ~set() = default;

  set(const set& s) : tree_(s.tree_) {}
  set(set&& s) noexcept : tree_(std::move(s.tree_)) {}

  iterator begin() noexcept { return tree_.cbegin(); }
  iterator end() noexcept { return tree_.cend(); }
  const_iterator begin() const noexcept { return tree_.cbegin(); }
  const_iterator end() const noexcept { return tree_.cend(); }
  const_iterator cbegin() const noexcept { return tree_.cbegin(); }
  const_iterator cend() const noexcept { return tree_.cend(); }
  set(std::initializer_list<value_type> const& keys) {
    for (const auto& key : keys) {
      tree_.insert(key, key);
    }
  }

  void erase(iterator pos) {
    if (pos != end()) {
      T key = (*pos).first;
      tree_.erase(key);
    }
  }
  void merge(set& other) {
    while (!other.empty()) {
      T value = (*other.begin()).first;
      auto [it, inserted] = insert(value);
      if (inserted) {
        other.erase(other.begin());
      }
    }
  }

  iterator find(const T& key) {
    auto node = tree_.search(key);
    return node ? iterator(node) : end();
  }

  bool empty() const { return tree_.empty(); }
  size_type size() const { return tree_.size(); }
  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }
  void clear() { tree_.clear(); }
  void swap(set& other) { tree_.swap(other.tree_); }

  std::pair<iterator, bool> insert(const value_type& value) {
    auto result = tree_.insert(value, value);
    return {iterator(result.first), result.second};
  }

  bool contains(const T& key) const { return tree_.search(key) != nullptr; }

  size_type count(const T& key) const { return contains(key) ? 1 : 0; }

  set& operator=(const set& other) {
    if (this != &other) {
      tree_ = other.tree_;
    }
    return *this;
  }
  set& operator=(set&& other) noexcept {
    if (this != &other) {
      tree_ = std::move(other.tree_);
    }
    return *this;
  }
  T operator*(const iterator& it) const { return it->first; }
};
}  // namespace s21

#endif  // S21_SET_H
