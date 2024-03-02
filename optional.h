#ifndef OPTIONAL_
#define OPTIONAL_

#include <stdexcept>
#include <type_traits>
#include <utility>

class BadOptionalAccess : std::exception {
 public:
  const char* what() const noexcept override {
    return "EmptyObject";
  }
  ~BadOptionalAccess() override = default;
};

template <typename T>
class Optional {
  char obj_[sizeof(T)];
  bool has_value_;
  T* Get();
  const T* Get() const;

 public:
  explicit Optional(bool has_value = false);
  Optional(const Optional&);      //  NOLINT
  Optional(Optional&&) noexcept;  //  NOLINT
  Optional(const T&);             //  NOLINT
  Optional(T&&);                  //  NOLINT
  Optional& operator=(const Optional&);
  Optional& operator=(Optional&&) noexcept;
  bool HasValue() const;
  ~Optional();
  operator bool() const;  //  NOLINT
  const T& Value() const;
  T& Value();
  const T& operator*() const;
  T& operator*();
  void Reset();
  template <typename... Args>
  void Emplace(Args&&... args);
  void Swap(Optional&);
};

template <typename T>
T* Optional<T>::Get() {
  return reinterpret_cast<T*>(obj_);
}

template <typename T>
const T* Optional<T>::Get() const {
  return reinterpret_cast<const T*>(obj_);
}

template <typename T>
Optional<T>::Optional(bool has_value) : has_value_(has_value) {
}

template <typename T>
Optional<T>::Optional(const Optional<T>& other) : Optional(false) {
  if (other.has_value_) {
    new (obj_) T(*other.Get());
    has_value_ = true;
  }
}

template <typename T>
Optional<T>::Optional(Optional<T>&& other) noexcept : Optional(false) {
  if (other.has_value_) {
    new (obj_) T(std::move(*other.Get()));
    has_value_ = true;
  }
}

template <typename T>
Optional<T>::Optional(const T& ptr) : Optional(true) {
  new (obj_) T(ptr);
}

template <typename T>
Optional<T>::Optional(T&& value) : Optional(true) {
  new (obj_) T(std::move(value));
}

template <typename T>
Optional<T>::~Optional() {
  Reset();
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional& other) {
  if (this != &other) {
    Reset();
    has_value_ = other.has_value_;
    if (has_value_) {
      new (obj_) T(*other.Get());
    }
  }
  return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(Optional&& other) noexcept {
  if (this != &other) {
    Reset();
    has_value_ = other.has_value_;
    if ((has_value_) && (this != &other)) {
      new (obj_) T(std::move(*other.Get()));
    }
  }
  return *this;
}

template <typename T>
bool Optional<T>::HasValue() const {
  return has_value_;
}

template <typename T>
Optional<T>::operator bool() const {
  return has_value_;
}

template <typename T>
const T& Optional<T>::Value() const {
  if (!has_value_) {
    throw BadOptionalAccess{};
  }
  return *Get();
}

template <typename T>
T& Optional<T>::Value() {
  if (!has_value_) {
    throw BadOptionalAccess{};
  }
  return *Get();
}

template <typename T>
T& Optional<T>::operator*() {
  return *Get();
}

template <typename T>
const T& Optional<T>::operator*() const {
  return *Get();
}

template <typename T>
void Optional<T>::Reset() {
  if (has_value_) {
    Get()->~T();
    has_value_ = false;
  }
}

template <typename T>
template <typename... Args>
void Optional<T>::Emplace(Args&&... args) {
  Reset();
  new (obj_) T(std::forward<Args>(args)...);
  has_value_ = true;
}

template <typename T>
void Optional<T>::Swap(Optional<T>& other) {
  std::swap(*this, other);
}

#endif
