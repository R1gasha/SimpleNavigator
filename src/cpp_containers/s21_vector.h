#ifndef S21_vector_H
#define S21_vector_H
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>

namespace s21 {
template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = size_t;

  vector();
  explicit vector(size_type n);
  vector(std::initializer_list<value_type> const& items);
  vector(const vector& v);
  vector(vector&& v) noexcept;
  ~vector();

  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;
  reference back();
  const_reference back() const;
  reference front();
  const_reference front() const;
  T* data();
  const T* data() const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const;
  size_type capacity() const noexcept;
  void reserve(size_type size);
  void shrink_to_fit();

  void clear() noexcept;
  iterator insert(iterator pos, const_reference val);
  iterator erase(const_iterator pos);
  void push_back(const_reference val);
  void pop_back() noexcept;
  void swap(vector& other) noexcept;

  vector& operator=(const vector& v);
  vector& operator=(vector&& v) noexcept;

 private:
  T* data_ = nullptr;
  size_type size_ = 0;
  size_type capacity_ = 0;
};

template <typename T>
vector<T>::vector() : data_(nullptr), size_(0), capacity_(0){};

template <typename T>
vector<T>::vector(size_type n)
    : data_(n > 0 ? new T[n]() : nullptr), size_(0), capacity_(n){};

template <typename T>
vector<T>::vector(std::initializer_list<value_type> const& items) {
  size_ = items.size();
  capacity_ = size_;
  if (size_ > 0) {
    data_ = new T[capacity_];
    std::copy(items.begin(), items.end(), data_);
  } else {
    data_ = nullptr;
  }
}

template <typename T>
vector<T>::vector(const vector& v) : size_(v.size_), capacity_(v.capacity_) {
  if (capacity_ > 0) {
    data_ = new T[capacity_];
    std::copy(v.data_, v.data_ + size_, data_);
  } else {
    data_ = nullptr;
  }
}

template <typename T>
vector<T>::vector(vector&& v) noexcept
    : data_(v.data_), size_(v.size_), capacity_(v.capacity_) {
  v.data_ = nullptr;
  v.size_ = 0;
  v.capacity_ = 0;
}

template <typename T>
vector<T>::~vector() {
  delete[] data_;
  size_ = 0;
  capacity_ = 0;
  data_ = nullptr;
}

template <typename T>
vector<T>& vector<T>::operator=(const vector& v) {
  if (this == &v) {
    return *this;
  }
  vector<T> temp(v);
  swap(temp);

  return *this;
}

template <typename T>
vector<T>& vector<T>::operator=(vector&& v) noexcept {
  if (this == &v) {
    return *this;
  }
  delete[] data_;
  data_ = v.data_;
  size_ = v.size_;
  capacity_ = v.capacity_;

  v.data_ = nullptr;
  v.size_ = 0;
  v.capacity_ = 0;

  return *this;
}

template <typename T>
typename vector<T>::reference vector<T>::at(size_type pos) {
  if (pos >= size_) {
    throw std::out_of_range("Индекс выходит за границы");
  }
  return data_[pos];
}  // LCOV_EXCL_LINE

template <typename T>
typename vector<T>::const_reference vector<T>::at(size_type pos) const {
  if (pos >= size_) {
    throw std::out_of_range("Индекс выходит за границы");
  }
  return data_[pos];
}  // LCOV_EXCL_LINE

template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type pos) {
  return data_[pos];
}

template <typename T>
typename vector<T>::const_reference vector<T>::operator[](size_type pos) const {
  return data_[pos];
}

template <typename T>
typename vector<T>::const_reference vector<T>::back() const {
  if (size_ == 0) {
    throw std::out_of_range("Индекс выходит за границы");
  }
  return data_[size_ - 1];
}  // LCOV_EXCL_LINE

template <typename T>
typename vector<T>::reference vector<T>::back() {
  if (size_ == 0) {
    throw std::out_of_range("Индекс выходит за границы");
  }
  return data_[size_ - 1];
}  // LCOV_EXCL_LINE

template <typename T>
typename vector<T>::const_reference vector<T>::front() const {
  if (size_ == 0) {
    throw std::out_of_range("Индекс выходит за границы");
  }
  return data_[0];
}  // LCOV_EXCL_LINE

template <typename T>
typename vector<T>::reference vector<T>::front() {
  if (size_ == 0) {
    throw std::out_of_range("Индекс выходит за границы");
  }
  return data_[0];
}  // LCOV_EXCL_LINE

template <typename T>
typename vector<T>::value_type* vector<T>::data() {
  return data_;
}

template <typename T>
const T* vector<T>::data() const {
  return data_;
}

template <typename T>
void vector<T>::clear() noexcept {
  size_ = 0;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos,
                                               const_reference val) {
  std::ptrdiff_t offset = pos - begin();
  if (pos < begin() || pos > end()) {
    throw std::out_of_range("Индекс выходит за границы");
  }
  value_type val_copy = val;
  if (size_ + 1 > capacity_) {
    size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
    reserve(new_capacity);
    pos = begin() + offset;
  }
  for (iterator i = end(); i > pos; --i) {
    *i = *(i - 1);
  }
  *pos = val_copy;
  size_++;

  return pos;
}

template <typename T>
void vector<T>::push_back(const_reference val) {
  if (size_ + 1 > capacity_) {
    size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
    reserve(new_capacity);
  }
  data_[size_] = val;
  ++size_;
}

template <typename T>
void vector<T>::pop_back() noexcept {
  if (size_ > 0) {
    size_--;
  }
}

template <typename T>
void vector<T>::swap(vector& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
  if (size_ == 0) {
    return end();
  }

  size_type offset = pos - begin();

  if (offset >= size_) {
    throw std::out_of_range("Индекс выходит за границы");
  }

  iterator mutable_pos = begin() + offset;

  for (iterator it = mutable_pos; it + 1 != end(); ++it) {
    *it = *(it + 1);
  }

  size_--;

  return begin() + offset;
}

template <typename T>
typename vector<T>::iterator vector<T>::begin() {
  return data_;
}

template <typename T>
typename vector<T>::iterator vector<T>::end() {
  return data_ + size_;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const {
  return data_;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
  return data_;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::end() const {
  return data_ + size_;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cend() const noexcept {
  return data_ + size_;
}

template <typename T>
void vector<T>::shrink_to_fit() {
  if (capacity_ > size_) {
    if (size_ == 0) {
      delete[] data_;
      data_ = nullptr;
      capacity_ = 0;
    } else {
      value_type* new_data = new T[size_];
      std::copy(data_, data_ + size_, new_data);
      delete[] data_;
      data_ = new_data;
      capacity_ = size_;
    }
  }
}

template <typename T>
bool vector<T>::empty() const noexcept {
  return size_ == 0 ? true : false;
}

template <typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
  return size_;
}

template <typename T>
typename vector<T>::size_type vector<T>::max_size() const {
  return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template <typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
  return capacity_;
}

template <typename T>
void vector<T>::reserve(size_type size) {
  if (size > capacity_) {
    value_type* new_data = new T[size]();
    if (data_) {
      std::copy(data_, data_ + size_, new_data);
      delete[] data_;
    }
    data_ = new_data;
    capacity_ = size;
  }
}
}  // namespace s21

#endif
