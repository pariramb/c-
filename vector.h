#ifndef VECTOR_H_
#define VECTOR_H_
#define VECTOR_MEMORY_IMPLEMENTED

#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <stdexcept>
#include <utility>

class VectorOutOfRange : public std::out_of_range {
 public:
  VectorOutOfRange() : std::out_of_range("VectorOutOfRange") {
  }
};

template <typename T>
class Vector {
 private:
  T* vector_;
  size_t size_;
  size_t capacity_;
  template <typename U = T>
  void PushBackHelperMove(size_t, U&&);
  template <typename... Args>
  void EmplaceBackHelper(size_t, Args&&...);

 public:
  Vector() noexcept : vector_(nullptr), size_(0), capacity_(0) {
  }
  explicit Vector(size_t);
  Vector(size_t, const T&);
  template <class It, class = std::enable_if_t<std::is_base_of<
                          std::forward_iterator_tag, typename std::iterator_traits<It>::iterator_category>::value>>
  Vector(It, It);
  Vector(std::initializer_list<T>);  // NOLINT
  Vector(const Vector&);             // NOLINT
  Vector(Vector&&) noexcept;         // NOLINT
  Vector& operator=(const Vector& other);
  Vector& operator=(Vector&& other) noexcept;
  size_t Size() const noexcept;
  size_t Capacity() const noexcept;
  bool Empty() const noexcept;
  T& operator[](size_t);
  const T& operator[](size_t) const;
  const T& At(size_t) const;
  T& At(size_t);
  T& Front() noexcept;
  const T& Front() const noexcept;
  T& Back() noexcept;
  const T& Back() const noexcept;
  T* Data() noexcept;
  const T* Data() const noexcept;
  void Swap(Vector&) noexcept;
  void Resize(size_t);
  void Resize(size_t, const T&);
  void Reserve(const size_t);
  void ShrinkToFit();
  void Clear();
  void PushBack(const T&);
  void PushBack(T&&);
  void PopBack();
  template <typename... Args>
  void EmplaceBack(Args&&... args);
  ~Vector();
  template <bool IsConst>
  class common_iterator;  // NOLINT
  using Iterator = common_iterator<false>;
  using ConstIterator = common_iterator<true>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  using Reference = T&;
  using ConstReference = const T&;
  using Pointer = T*;
  using ConstPointer = const T*;
  using ValueType = T;
  using SizeType = size_t;

  Iterator begin();  // NOLINT

  ConstIterator begin() const;  // NOLINT

  Iterator end();  // NOLINT

  ConstIterator end() const;             // NOLINT
  ConstIterator cbegin() const;          // NOLINT
  ConstIterator cend() const;            // NOLINT
  ReverseIterator rbegin();              // NOLINT
  ConstReverseIterator rbegin() const;   // NOLINT
  ReverseIterator rend();                // NOLINT
  ConstReverseIterator rend() const;     // NOLINT
  ConstReverseIterator crbegin() const;  // NOLINT
  ConstReverseIterator crend() const;    // NOLINT
};

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin() {  // NOLINT
  return Vector<T>::Iterator(vector_);
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::begin() const {  // NOLINT
  return Vector<T>::ConstIterator(vector_);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end() {  // NOLINT
  return Vector<T>::Iterator(vector_ + size_);
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::end() const {  // NOLINT
  return Vector<T>::ConstIterator(vector_ + size_);
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::cbegin() const {  // NOLINT
  return begin();
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::cend() const {  // NOLINT
  return end();
}

template <typename T>
typename Vector<T>::ReverseIterator Vector<T>::rbegin() {  // NOLINT
  return std::make_reverse_iterator(end());
}

template <typename T>
typename Vector<T>::ConstReverseIterator Vector<T>::rbegin() const {  // NOLINT
  return std::make_reverse_iterator(end());
}

template <typename T>
typename Vector<T>::ReverseIterator Vector<T>::rend() {  // NOLINT
  return std::make_reverse_iterator(begin());
}

template <typename T>
typename Vector<T>::ConstReverseIterator Vector<T>::rend() const {  // NOLINT
  return std::make_reverse_iterator(begin());
}

template <typename T>
typename Vector<T>::ConstReverseIterator Vector<T>::crbegin() const {  // NOLINT
  return rbegin();
}

template <typename T>
typename Vector<T>::ConstReverseIterator Vector<T>::crend() const {  // NOLINT
  return rend();
}

template <typename T>
void TransferArray(T* vector1, T* vector2, size_t size) {
  size_t index = 0;
  try {
    for (; index < size; ++index) {
      new (vector1 + index) T(vector2[index]);
    }
  } catch (...) {
    for (size_t i = 0; i < index; ++i) {
      vector1[i].~T();
    }
    throw;
  }
}

template <typename T>
void TransferArrayMove(T* vector1, T* vector2, size_t size) {
  size_t index = 0;
  try {
    for (; index < size; ++index) {
      new (vector1 + index) T(std::move_if_noexcept(vector2[index]));
    }
  } catch (...) {
    for (; index < size; ++index) {
      vector1[index].~T();
    }
  }
}

template <typename T>
void Vector<T>::Reserve(size_t new_cap) {
  if (capacity_ < new_cap) {
    auto vector_temp = reinterpret_cast<T*>(operator new(new_cap * sizeof(T)));
    try {
      TransferArrayMove(vector_temp, vector_, size_);
    } catch (...) {
      operator delete(vector_temp);
      throw;
    }
    if (!noexcept(T(std::move(vector_temp[0])))) {
      for (size_t i = 0; i < size_; ++i) {
        vector_[i].~T();
      }
    }
    operator delete(vector_);
    capacity_ = new_cap;
    vector_ = vector_temp;
  }
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
  if (other.capacity_ != 0) {
    vector_ = reinterpret_cast<T*>(operator new(other.capacity_ * sizeof(T)));
    try {
      TransferArray(vector_, other.vector_, other.size_);
    } catch (...) {
      operator delete(vector_);
      throw;
    }
  } else {
    vector_ = nullptr;
  }
  size_ = other.size_;
  capacity_ = other.capacity_;
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept : vector_(other.vector_), size_(other.size_), capacity_(other.capacity_) {
  other.vector_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T>
template <class It, class>
Vector<T>::Vector(It it_begin, It it_end) {
  size_t size = 0;
  for (auto it = it_begin; it != it_end; ++it) {
    ++size;
  }
  if (size != 0) {
    size_t index = 0;
    vector_ = reinterpret_cast<T*>(operator new(size * sizeof(T)));
    try {
      for (auto it = it_begin; index < size; ++index, ++it) {
        new (vector_ + index) T(*it);
      }
    } catch (...) {
      for (size_t i = 0; i < index; ++i) {
        vector_[i].~T();
      }
      operator delete(vector_);
      throw;
    }
  } else {
    vector_ = nullptr;
  }
  capacity_ = size_ = size;
}

template <typename T>
Vector<T>::Vector(size_t size) {
  if (size != 0) {
    vector_ = reinterpret_cast<T*>(operator new(size * sizeof(T)));
    size_t index = 0;
    try {
      for (; index < size; ++index) {
        new (vector_ + index) T();
      }
    } catch (...) {
      for (size_t i = 0; i < index; ++i) {
        vector_[i].~T();
      }
      operator delete(vector_);
      throw;
    }
  } else {
    vector_ = nullptr;
  }
  capacity_ = size_ = size;
}

template <typename T>
Vector<T>::Vector(size_t size, const T& value) {
  if (size != 0) {
    vector_ = reinterpret_cast<T*>(operator new(size * sizeof(T)));
    size_t index = 0;
    try {
      for (; index < size; ++index) {
        new (vector_ + index) T(value);
      }
    } catch (...) {
      for (size_t i = 0; i < index; ++i) {
        vector_[i].~T();
      }
      operator delete(vector_);
      throw;
    }
  } else {
    vector_ = nullptr;
  }
  capacity_ = size_ = size;
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list) : Vector(list.begin(), list.end()) {
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
  try {
    Vector<T> copy(other);
    Swap(copy);
  } catch (...) {
    Vector<T> copy = std::move(Vector());
    Swap(copy);
    throw;
  }
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
  auto copy(std::move(other));
  Swap(copy);
  return *this;
}

template <typename T>
size_t Vector<T>::Size() const noexcept {
  return size_;
}

template <typename T>
size_t Vector<T>::Capacity() const noexcept {
  return capacity_;
}

template <typename T>
bool Vector<T>::Empty() const noexcept {
  return (size_ == 0);
}

template <typename T>
T& Vector<T>::operator[](size_t index) {
  return *(vector_ + index);
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const {
  return *(vector_ + index);
}

template <typename T>
const T& Vector<T>::At(size_t index) const {
  if (index >= size_) {
    throw VectorOutOfRange{};
  }
  return *(vector_ + index);
}

template <typename T>
T& Vector<T>::At(size_t index) {
  if (index >= size_) {
    throw VectorOutOfRange{};
  }
  return *(vector_ + index);
}

template <typename T>
T& Vector<T>::Front() noexcept {
  return *vector_;
}

template <typename T>
const T& Vector<T>::Front() const noexcept {
  return *vector_;
}

template <typename T>
T& Vector<T>::Back() noexcept {
  return vector_[Size() - 1];
}

template <typename T>
const T& Vector<T>::Back() const noexcept {
  return vector_[Size() - 1];
}

template <typename T>
T* Vector<T>::Data() noexcept {
  return vector_;
}

template <typename T>
const T* Vector<T>::Data() const noexcept {
  return vector_;
}

template <typename T>
void Vector<T>::Swap(Vector& other) noexcept {
  std::swap(vector_, other.vector_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T>
void Vector<T>::Resize(size_t new_size) {
  if (capacity_ >= new_size) {
    if (size_ >= new_size) {
      for (size_t i = new_size; i < size_; ++i) {
        vector_[i].~T();
      }
      size_ = new_size;
      return;
    }
    size_t index = size_;
    try {
      for (; index < new_size; ++index) {
        new (vector_ + index) T();
      }
    } catch (...) {
      for (size_t i = size_; i < index; ++i) {
        vector_[i].~T();
      }
      throw;
    }
    size_ = new_size;
    return;
  }
  size_t new_cap = (!Capacity() ? 1 : capacity_);
  while (new_size > new_cap) {
    new_cap *= 2;
  }
  auto vector_temp = reinterpret_cast<T*>(operator new(new_cap * sizeof(T)));
  if (size_ < new_size) {
    size_t index = size_;
    try {
      for (; index < new_size; ++index) {
        new (vector_temp + index) T();
      }
    } catch (...) {
      for (size_t i = size_; i < index; ++i) {
        vector_temp[i].~T();
      }
      operator delete(vector_temp);
      throw;
    }
    try {
      TransferArrayMove(vector_temp, vector_, size_);
    } catch (...) {
      for (size_t i = size_; i < index; ++i) {
        vector_temp[i].~T();
      }
      operator delete(vector_temp);
      throw;
    }
    if (!noexcept(T(std::move(vector_temp[0])))) {
      for (size_t i = 0; i < size_; ++i) {
        vector_[i].~T();
      }
    }
    if (capacity_) {
      operator delete(vector_);
    }
    size_ = new_size;
    capacity_ = new_cap;
    vector_ = vector_temp;
  }
}

template <typename T>
void Vector<T>::Resize(size_t new_size, const T& value) {
  if (capacity_ >= new_size) {
    if (size_ >= new_size) {
      for (size_t i = new_size; i < size_; ++i) {
        vector_[i].~T();
      }
      size_ = new_size;
      return;
    }
    size_t index = size_;
    try {
      for (; index < new_size; ++index) {
        new (vector_ + index) T(value);
      }
    } catch (...) {
      for (size_t i = size_; i < index; ++i) {
        vector_[i].~T();
      }
      throw;
    }
    size_ = new_size;
    return;
  }
  size_t new_cap = (!Capacity() ? 1 : capacity_);
  while (new_size > new_cap) {
    new_cap *= 2;
  }
  auto vector_temp = reinterpret_cast<T*>(operator new(new_cap * sizeof(T)));
  if (size_ < new_size) {
    size_t index = size_;
    try {
      for (; index < new_size; ++index) {
        new (vector_temp + index) T(value);
      }
    } catch (...) {
      for (size_t i = size_; i < index; ++i) {
        vector_temp[i].~T();
      }
      operator delete(vector_temp);
      throw;
    }
    try {
      TransferArrayMove(vector_temp, vector_, size_);
    } catch (...) {
      for (size_t i = size_; i < index; ++i) {
        vector_temp[i].~T();
      }
      operator delete(vector_temp);
      throw;
    }
    if (!noexcept(T(std::move(vector_temp[0])))) {
      for (size_t i = 0; i < size_; ++i) {
        vector_[i].~T();
      }
    }
    if (capacity_) {
      operator delete(vector_);
    }
    size_ = new_size;
    capacity_ = new_cap;
    vector_ = vector_temp;
  }
}

template <typename T>
void Vector<T>::ShrinkToFit() {
  if (capacity_ != size_) {
    auto temp = (Empty() ? nullptr : reinterpret_cast<T*>(operator new(size_ * sizeof(T))));
    try {
      TransferArrayMove(temp, vector_, size_);
    } catch (...) {
      operator delete(temp);
      throw;
    }
    if (!noexcept(T(std::move(vector_[0])))) {
      for (size_t i = 0; i < size_; ++i) {
        vector_[i].~T();
      }
    }
    if (vector_) {
      operator delete(vector_);
    }
    vector_ = temp;
    capacity_ = size_;
  }
}

template <typename T>
void Vector<T>::Clear() {
  for (size_t i = 0; i < size_; ++i) {
    vector_[i].~T();
  }
  size_ = 0;
}

template <typename T>
void Vector<T>::PushBack(const T& value) {
  if (capacity_ == 0) {
    vector_ = reinterpret_cast<T*>(operator new(sizeof(T)));
    capacity_ = 1;
  }
  if (size_ == capacity_) {
    try {
      PushBackHelperMove(capacity_ * 2, value);
    } catch (...) {
      if (capacity_ == 1) {
        operator delete(vector_);
        capacity_ = 0;
      }
      throw;
    }
  } else {
    new (vector_ + size_) T(value);
    size_ += 1;
  }
}

template <typename T>
template <typename U>
void Vector<T>::PushBackHelperMove(size_t new_cap, U&& value) {
  auto vector_temp = reinterpret_cast<T*>(operator new(new_cap * sizeof(T)));
  try {
    TransferArrayMove(vector_temp, vector_, size_);
  } catch (...) {
    operator delete(vector_temp);
    throw;
  }
  try {
    new (vector_temp + size_) T(std::move_if_noexcept(std::forward<U>(value)));
  } catch (...) {
    for (size_t i = 0; i < size_; ++i) {
      vector_temp[i].~T();
    }
    operator delete(vector_temp);
    throw;
  }
  if (!noexcept(T(std::move(vector_temp[0])))) {
    for (size_t i = 0; i < size_; ++i) {
      vector_[i].~T();
    }
  }
  operator delete(vector_);
  capacity_ = new_cap;
  vector_ = vector_temp;
  size_ += 1;
}

template <typename T>
void Vector<T>::PushBack(T&& value) {
  if (capacity_ == 0) {
    vector_ = reinterpret_cast<T*>(operator new(sizeof(T)));
    capacity_ = 1;
  }
  if (size_ == capacity_) {
    try {
      PushBackHelperMove(capacity_ * 2, value);
    } catch (...) {
      if (capacity_ == 1) {
        operator delete(vector_);
        capacity_ = 0;
      }
      throw;
    }
  } else {
    new (vector_ + size_) T(std::move(value));
    size_ += 1;
  }
}

template <typename T>
void Vector<T>::PopBack() {
  vector_[size_ - 1].~T();
  --size_;
}

template <typename T>
template <typename... Args>
void Vector<T>::EmplaceBackHelper(size_t new_cap, Args&&... args) {
  auto vector_temp = reinterpret_cast<T*>(operator new(new_cap * sizeof(T)));
  try {
    TransferArrayMove(vector_temp, vector_, size_);
  } catch (...) {
    operator delete(vector_temp);
    throw;
  }
  try {
    new (vector_temp + size_) T(std::forward<Args>(args)...);
  } catch (...) {
    for (size_t i = 0; i < size_ + 1; ++i) {
      vector_temp[i].~T();
    }
    operator delete(vector_temp);
    throw;
  }
  if (!noexcept(T(std::move(vector_temp[0])))) {
    for (size_t i = 0; i < size_; ++i) {
      vector_[i].~T();
    }
  }
  operator delete(vector_);
  capacity_ = new_cap;
  vector_ = vector_temp;
  size_ += 1;
}

template <typename T>
template <typename... Args>
void Vector<T>::EmplaceBack(Args&&... args) {
  if (capacity_ == 0) {
    vector_ = reinterpret_cast<T*>(operator new(sizeof(T)));
    capacity_ = 1;
  }
  if (size_ == capacity_) {
    try {
      EmplaceBackHelper(capacity_ * 2, std::forward<Args>(args)...);
    } catch (...) {
      if (capacity_ == 1) {
        operator delete(vector_);
        capacity_ = 0;
      }
    }
  } else {
    new (vector_ + size_) T(std::forward<Args>(args)...);
    size_ += 1;
  }
}

template <typename T>
Vector<T>::~Vector() {
  Clear();
  if (capacity_ != 0) {
    operator delete(vector_);
    capacity_ = 0;
  }
}

template <typename T>
bool operator<(const Vector<T>& vector1, const Vector<T>& vector2) {
  size_t min_size = std::min(vector1.Size(), vector2.Size());
  for (size_t i = 0; i < min_size; ++i) {
    if (vector1[i] != vector2[i]) {
      return vector1[i] < vector2[i];
    }
  }
  return vector1.Size() < vector2.Size();
}

template <typename T>
bool operator>=(const Vector<T>& vector1, const Vector<T>& vector2) {
  return !(vector1 < vector2);
}

template <typename T>
bool operator==(const Vector<T>& vector1, const Vector<T>& vector2) {
  return !(vector1 < vector2) && !(vector2 < vector1);
}

template <typename T>
bool operator<=(const Vector<T>& vector1, const Vector<T>& vector2) {
  return !(vector1 > vector2);
}

template <typename T>
bool operator>(const Vector<T>& vector1, const Vector<T>& vector2) {
  return vector2 < vector1;
}

template <typename T>
bool operator!=(const Vector<T>& vector1, const Vector<T>& vector2) {
  return !(vector1 == vector2);
}

template <typename T>
template <bool IsConst>
class Vector<T>::common_iterator {  // NOLINT
 private:
  using Type = std::conditional_t<IsConst, const T, T>;
  Type* vector_;
  explicit common_iterator(Type* ptr) : vector_(ptr) {
  }

 public:
  using reference = Type&;                                    // NOLINT
  using pointer = Type*;                                      // NOLINT
  using iterator_category = std::random_access_iterator_tag;  // NOLINT
  using value_type = T;                                       // NOLINT
  using difference_type = int64_t;                            // NOLINT
  friend class Vector;

  Type& operator*() {
    return *vector_;
  }

  common_iterator& operator++() {
    ++vector_;
    return *this;
  }

  common_iterator operator++(int) {
    Type* temp = vector_;
    ++vector_;
    return common_iterator(temp);
  }

  common_iterator& operator--() {
    --vector_;
    return *this;
  }

  common_iterator operator--(int) {
    Type* temp = vector_;
    --vector_;
    return common_iterator(temp);
  }

  Type* operator->() const {
    return vector_;
  }

  common_iterator& operator+=(int64_t n) {
    vector_ = vector_ + n;
    return *this;
  }

  common_iterator& operator-=(int64_t n) {
    return *this += -n;
  }
  Type& operator[](size_t index) const {
    return vector_[index];
  }

  template <bool IsConstNew>
  int operator-(const common_iterator<IsConstNew>& it) const;

  template <bool IsConstNew>
  bool operator<(const common_iterator<IsConstNew>&) const;

  template <bool IsConstNew>
  bool operator==(const common_iterator<IsConstNew>& it) const;

  template <bool IsConstNew>
  bool operator>=(const common_iterator<IsConstNew>& it) const;

  template <bool IsConstNew>
  bool operator>(const common_iterator<IsConstNew>& it) const;

  template <bool IsConstNew>
  bool operator<=(const common_iterator<IsConstNew>& it) const;

  template <bool IsConstNew>
  bool operator!=(const common_iterator<IsConstNew>& it) const;
};

template <class T>
template <bool IsConst>
template <bool IsConstNew>
int Vector<T>::common_iterator<IsConst>::operator-(const Vector<T>::common_iterator<IsConstNew>& it) const {
  return (vector_ - it.vector_);
}

template <typename CommonIterator>
std::remove_const_t<CommonIterator> operator+(const CommonIterator& it, int n) {
  std::remove_const_t<CommonIterator> copy = it;
  return copy += n;
}

template <typename CommonIterator>
std::remove_const_t<CommonIterator> operator+(int n, const CommonIterator& it) {
  return it + n;
}

template <typename T, bool IsConst>
typename Vector<T>::template common_iterator<IsConst> operator-(
    const typename Vector<T>::template common_iterator<IsConst>& it, int n) {
  return it + (-n);
}

template <typename T>
template <bool IsConst>
template <bool IsConstNew>
bool Vector<T>::common_iterator<IsConst>::operator<(
    const typename Vector<T>::template common_iterator<IsConstNew>& it) const {
  return ((*this - it) < 0);
}

template <typename T>
template <bool IsConst>
template <bool IsConstNew>
bool Vector<T>::common_iterator<IsConst>::operator==(
    const typename Vector<T>::template common_iterator<IsConstNew>& it) const {
  return ((*this - it) == 0);
}

template <typename T>
template <bool IsConst>
template <bool IsConstNew>
bool Vector<T>::common_iterator<IsConst>::operator>=(
    const typename Vector<T>::template common_iterator<IsConstNew>& it) const {
  return !(*this < it);
}

template <typename T>
template <bool IsConst>
template <bool IsConstNew>
bool Vector<T>::common_iterator<IsConst>::operator>(
    const typename Vector<T>::template common_iterator<IsConstNew>& it) const {
  return (!(*this < it) && !(*this == it));
}

template <typename T>
template <bool IsConst>
template <bool IsConstNew>
bool Vector<T>::common_iterator<IsConst>::operator<=(
    const typename Vector<T>::template common_iterator<IsConstNew>& it) const {
  return !(*this > it);
}

template <typename T>
template <bool IsConst>
template <bool IsConstNew>
bool Vector<T>::common_iterator<IsConst>::operator!=(
    const typename Vector<T>::template common_iterator<IsConstNew>& it) const {
  return !(*this == it);
}

#endif
