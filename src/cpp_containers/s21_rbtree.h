#ifndef S21_RBTREE_H
#define S21_RBTREE_H
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace s21 {
// LCOV_EXCL_START
template <typename K, typename V>
struct RBNode {
  using pair_type = std::pair<K, V>;
  pair_type data;
  RBNode* parent;
  RBNode* left;
  RBNode* right;
  bool is_red;

  RBNode(const K& k, const V& v)
      : data(k, v),
        parent(nullptr),
        left(nullptr),
        right(nullptr),
        is_red(true) {}
};
template <typename K, typename V, typename Compare = std::less<K>>
class RBTree {
 public:
  template <bool IsConst>
  friend class RBTreeIterator;
  using key_type = K;
  using value_type = V;
  using pair_type = std::pair<const K, V>;
  using node_type = RBNode<K, V>;
  using size_type = size_t;
  template <bool IsConst>
  class RBTreeIterator {
   public:
    using node_type = RBNode<K, V>;
    using value_type = std::pair<const K, V>;
    using reference = typename std::conditional<IsConst, const value_type&,
                                                value_type&>::type;
    using pointer = typename std::conditional<IsConst, const value_type*,
                                              value_type*>::type;
    // LCOV_EXCL_STOP
    RBTreeIterator(node_type* node = nullptr) : node_(node) {}
    bool operator==(const RBTreeIterator& other) const {
      return node_ == other.node_;
    }
    bool operator!=(const RBTreeIterator& other) const {
      return !(*this == other);
    }
    pointer operator->() const noexcept { return &(node_->data); }
    RBTreeIterator& operator++() noexcept {
      if (node_ == nullptr) return *this;
      node_ = next_node(node_);
      return *this;
    }
    RBTreeIterator operator++(int) noexcept {
      RBTreeIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    node_type* node() const { return node_; }

    value_type operator*() const noexcept {
      return {node_->data.first, node_->data.second};
    }

   private:
    node_type* node_;

    static node_type* next_node(node_type* current) {
      if (current == nullptr) return nullptr;
      if (current->right != nullptr) {
        current = current->right;
        while (current->left != nullptr) current = current->left;
        return current;
      }
      while (current->parent != nullptr && current == current->parent->right) {
        current = current->parent;
      }
      return current->parent;
    }
  };

 public:
  using iterator = RBTreeIterator<false>;
  using const_iterator = RBTreeIterator<true>;

  iterator begin() noexcept { return iterator(search_min()); }
  iterator end() noexcept { return iterator(nullptr); }
  const_iterator cbegin() const noexcept {
    return const_iterator(search_min());
  }
  const_iterator cend() const noexcept { return const_iterator(nullptr); }
  const_iterator begin() const noexcept { return cbegin(); }
  const_iterator end() const noexcept { return cend(); }

  RBTree() : root_(nullptr), size_(0), comp_() {}

  ~RBTree() { clear(); }

  RBTree(const RBTree& other) : root_(nullptr), size_(0), comp_(other.comp_) {
    if (other.root_) {
      root_ = copy_tree(other.root_, nullptr);
      size_ = other.size_;
    }
  }

  RBTree(RBTree&& other) noexcept
      : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)) {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  void clear() {
    clear_recursive(root_);
    root_ = nullptr;
    size_ = 0;
  }

  RBTree& operator=(RBTree&& other) noexcept {
    if (this != &other) {
      clear();
      root_ = other.root_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);

      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  RBTree& operator=(const RBTree& other) {
    if (this != &other) {
      clear();

      if (other.root_) {
        root_ = copy_tree(other.root_, nullptr);
        size_ = other.size_;
        comp_ = other.comp_;
      } else {
        root_ = nullptr;
        size_ = 0;
      }
    }
    return *this;
  }

  node_type* search(const K& key) {
    node_type* current = root_;
    while (current != nullptr) {
      if (Compare()(key, current->data.first)) {
        current = current->left;
      } else if (Compare()(current->data.first, key)) {
        current = current->right;
      } else {
        return current;
      }
    }
    return nullptr;
  }

  node_type* search(const K& key) const {
    node_type* current = root_;
    while (current != nullptr) {
      if (comp_(key, current->data.first)) {
        current = current->left;
      } else if (comp_(current->data.first, key)) {
        current = current->right;
      } else {
        return current;
      }
    }
    return nullptr;
  }

  std::pair<node_type*, bool> insert(const K& key, const V& value = V()) {
    if (root_ == nullptr) {
      root_ = new node_type(key, value);
      root_->is_red = false;
      size_++;
      return {root_, true};
    }

    node_type* parent = nullptr;
    node_type* current = root_;

    while (current != nullptr) {
      parent = current;
      if (Compare()(key, current->data.first)) {
        current = current->left;
      } else if (Compare()(current->data.first, key)) {
        current = current->right;
      } else {
        return {current, false};
      }
    }

    node_type* new_node = new node_type(key, value);
    new_node->parent = parent;

    if (Compare()(key, parent->data.first)) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    size_++;
    if (new_node->parent->is_red) {
      rebalance(new_node);
    }

    return {new_node, true};
  }

  node_type* search_min() const {
    if (root_ == nullptr) return nullptr;

    node_type* current = root_;
    while (current->left != nullptr) {
      current = current->left;
    }
    return current;
  }

  node_type* search_max() const {
    if (root_ == nullptr) return nullptr;

    node_type* current = root_;
    while (current->right != nullptr) {
      current = current->right;
    }
    return current;
  }

  node_type* next(node_type* current) {
    if (current == nullptr) return nullptr;

    if (current->right != nullptr) {
      current = current->right;
      while (current->left != nullptr) {
        current = current->left;
      }
      return current;
    }

    while (current->parent != nullptr && current == current->parent->right) {
      current = current->parent;
    }

    return current->parent;
  }

  node_type* prev(node_type* current) {
    if (current == nullptr) return nullptr;

    if (current->left != nullptr) {
      current = current->left;
      while (current->right != nullptr) {
        current = current->right;
      }
      return current;
    }

    while (current->parent != nullptr && current == current->parent->left) {
      current = current->parent;
    }

    return current->parent;
  }

  bool erase(const K& key) {
    node_type* node = search(key);
    if (node == nullptr) {
      return false;
    }
    erase_node(node);
    size_--;
    return true;
  }

  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }

  node_type* root() const { return root_; }

  void swap(RBTree& other) noexcept {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);
  }

 private:
  node_type* root_;
  size_type size_;
  Compare comp_;

  void rebalance(node_type* node) {
    while (node->parent != nullptr && node->parent->is_red) {
      node_type* grandpa = node->parent->parent;
      if (grandpa == nullptr) return;
      if (grandpa->left == nullptr || grandpa->right == nullptr) return;

      if (node->parent == grandpa->left) {
        node_type* uncle = grandpa->right;
        if (uncle != nullptr && uncle->is_red) {
          node->parent->is_red = false;
          uncle->is_red = false;
          grandpa->is_red = true;
          node = grandpa;
        } else {
          if (node == node->parent->right) {
            node = node->parent;
            rotate_left(node);
          }
          node->parent->is_red = false;
          grandpa->is_red = true;
          rotate_right(grandpa);
        }
      } else {
        node_type* uncle = grandpa->left;
        if (uncle != nullptr && uncle->is_red) {
          node->parent->is_red = false;
          uncle->is_red = false;
          grandpa->is_red = true;
          node = grandpa;
        } else {
          if (node == node->parent->left) {
            node = node->parent;
            rotate_right(node);
          }
          node->parent->is_red = false;
          grandpa->is_red = true;
          rotate_left(grandpa);
        }
      }
    }
  }

  void clear_recursive(node_type* node) {
    if (node == nullptr) return;

    clear_recursive(node->right);
    clear_recursive(node->left);
    delete node;
  }

  void erase_node(node_type* node) {
    if (node == nullptr) return;

    if (node->left != nullptr && node->right != nullptr) {
      node_type* successor = node->right;
      while (successor->left != nullptr) {
        successor = successor->left;
      }

      node->data = successor->data;
      node = successor;
    }

    node_type* child = (node->left != nullptr) ? node->left : node->right;
    bool node_is_black = !node->is_red;

    if (node_is_black) {
      if (child != nullptr) {
        child->is_red = false;
      } else {
        rebalance_erase(node);
      }
    }

    if (child != nullptr) {
      child->parent = node->parent;
    }

    if (node->parent == nullptr) {
      root_ = child;
    } else if (node == node->parent->left) {
      node->parent->left = child;
    } else {
      node->parent->right = child;
    }

    delete node;
  }

  void rebalance_erase(node_type* node) {
    while (node != nullptr && node != root_ && !node->is_red) {
      node_type* parent = node->parent;
      if (node == parent->left) {
        node_type* sibling = parent->right;
        if (sibling->is_red) {
          sibling->is_red = false;
          parent->is_red = true;
          rotate_left(parent);
          sibling = parent->right;
        }
        bool left_black = (sibling->left == nullptr || !sibling->left->is_red);
        bool right_black =
            (sibling->right == nullptr || !sibling->right->is_red);
        if (left_black && right_black) {
          sibling->is_red = true;
          node = parent;
        } else {
          if (sibling->right == nullptr || !sibling->right->is_red) {
            if (sibling->left != nullptr) {
              sibling->left->is_red = false;
            }
            sibling->is_red = true;
            rotate_right(sibling);
            sibling = parent->right;
          }
          sibling->is_red = parent->is_red;
          parent->is_red = false;
          if (sibling->right != nullptr) {
            sibling->right->is_red = false;
          }
          rotate_left(parent);
          node = root_;
        }
      } else {
        node_type* sibling = parent->left;
        if (sibling->is_red) {
          sibling->is_red = false;
          parent->is_red = true;
          rotate_right(parent);
          sibling = parent->left;
        }
        bool left_black = (sibling->left == nullptr || !sibling->left->is_red);
        bool right_black =
            (sibling->right == nullptr || !sibling->right->is_red);
        if (left_black && right_black) {
          sibling->is_red = true;
          node = parent;
        } else {
          if (sibling->left == nullptr || !sibling->left->is_red) {
            if (sibling->right != nullptr) {
              sibling->right->is_red = false;
            }
            sibling->is_red = true;
            rotate_left(sibling);
            sibling = parent->left;
          }
          sibling->is_red = parent->is_red;
          parent->is_red = false;
          if (sibling->left != nullptr) {
            sibling->left->is_red = false;
          }
          rotate_right(parent);
          node = root_;
        }
      }
    }

    if (node != nullptr) {
      node->is_red = false;
    }
  }

  void rotate_right(node_type* node) {
    node_type* left_child = node->left;
    node->left = left_child->right;

    if (left_child->right != nullptr) {
      left_child->right->parent = node;
    }

    left_child->parent = node->parent;

    if (node->parent == nullptr) {
      root_ = left_child;
    } else if (node == node->parent->right) {
      node->parent->right = left_child;
    } else {
      node->parent->left = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
  }

  void rotate_left(node_type* node) {
    node_type* right_child = node->right;
    node->right = right_child->left;

    if (right_child->left != nullptr) {
      right_child->left->parent = node;
    }

    right_child->parent = node->parent;

    if (node->parent == nullptr) {
      root_ = right_child;
    } else if (node == node->parent->left) {
      node->parent->left = right_child;
    } else {
      node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
  }

  node_type* copy_tree(node_type* node, node_type* parent) {
    if (!node) return nullptr;

    node_type* new_node = new node_type(node->data.first, node->data.second);
    new_node->is_red = node->is_red;
    new_node->parent = parent;

    new_node->left = copy_tree(node->left, new_node);
    new_node->right = copy_tree(node->right, new_node);

    return new_node;
  }
};

}  // namespace s21

#endif
