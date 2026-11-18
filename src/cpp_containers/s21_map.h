#ifndef S21_MAP_H
#define S21_MAP_H
#include "s21_rbtree.h"

namespace s21 {
template <typename Key, typename Value>
class map {
 private:
  s21::RBTree<Key, Value> tree_;

 public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename s21::RBTree<Key, Value>::iterator;
  using const_iterator = typename s21::RBTree<Key, Value>::const_iterator;
  using size_type = std::size_t;

  map() = default;
  ~map() = default;
  map(std::initializer_list<value_type> const& items) {
    for (const auto& item : items) {
      tree_.insert(item.first, item.second);
    }
  }
  map(const map& m) : tree_(m.tree_) {}
  map(map&& m) noexcept : tree_(std::move(m.tree_)) {}

  map& operator=(const map& other) {
    if (this != &other) {
      tree_ = other.tree_;
    }
    return *this;
  }

  map& operator=(map&& other) noexcept {
    if (this != &other) {
      tree_ = std::move(other.tree_);
    }
    return *this;
  }

  mapped_type& at(const Key& key) {
    auto node = tree_.search(key);
    if (node == nullptr) {
      throw std::out_of_range("map::at: key not found");
    }
    return node->data.second;
  }  // LCOV_EXCL_LINE

  iterator find(const Key& key) {
    auto node = tree_.search(key);
    return node ? iterator(node) : tree_.end();
  }

  mapped_type& operator[](const Key& key) {
    auto result = tree_.insert(key, mapped_type());
    return result.first->data.second;
  }  // LCOV_EXCL_LINE

  iterator begin() noexcept { return tree_.begin(); }

  iterator end() noexcept { return tree_.end(); }

  bool empty() const { return tree_.empty(); }

  size_type size() const { return tree_.size(); }
  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }
  void clear() { tree_.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) {
    auto result = tree_.insert(value.first, value.second);
    return {iterator(result.first), result.second};
  }

  std::pair<iterator, bool> insert(const Key& key, const mapped_type& obj) {
    auto result = tree_.insert(key, obj);
    return {iterator(result.first), result.second};
  }

  std::pair<iterator, bool> insert_or_assign(const Key& key,
                                             const mapped_type& obj) {
    auto node = tree_.search(key);
    if (!node) {
      auto result = tree_.insert(key, obj);
      return {iterator(result.first), true};
    } else {
      node->data.second = obj;
      return {iterator(node), false};
    }
  }

  void erase(iterator pos) {
    if (auto* node = pos.node()) {
      tree_.erase(node->data.first);
    }
  }

  bool erase(const Key& key) { return tree_.erase(key); }

  void swap(map& other) { tree_.swap(other.tree_); }

  void merge(map& other) {
    while (!other.empty()) {
      Key key = (*other.begin()).first;
      mapped_type value = other.at(key);
      insert(key, value);
      other.erase(key);
    }
  }

  bool contains(const Key& key) { return tree_.search(key) != nullptr; }

  const_iterator begin() const noexcept { return tree_.cbegin(); }
  const_iterator end() const noexcept { return tree_.cend(); }
  const_iterator cbegin() const noexcept { return tree_.cbegin(); }
  const_iterator cend() const noexcept { return tree_.cend(); }

  const_iterator find(const Key& key) const {
    auto node = tree_.search(key);
    return node ? const_iterator(node) : cend();
  }

};  // S21_MAP
}  // namespace s21

#endif
