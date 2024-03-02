#ifndef SHARED_PTR_
#define SHARED_PTR_
#define WEAK_PTR_IMPLEMENTED

#include <stdexcept>
#include <iostream>
#include <stdexcept>

template <class T>
class SharedPtr;
template <class T>
class WeakPtr;
struct Counter;

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

struct Counter {
  size_t strong_counter;
  size_t weak_counter;
  Counter(const size_t strong, const size_t weak) : strong_counter(strong), weak_counter(weak) {
  }
};

template <class T>
class SharedPtr {
 private:
  T* shared_ptr_;
  Counter* counter_;

 public:
  SharedPtr();
  explicit SharedPtr(const WeakPtr<T>& weak_ptr);
  SharedPtr(T* ptr);                     // NOLINT
  SharedPtr(const SharedPtr<T>& other);  // NOLINT
  SharedPtr& operator=(const SharedPtr& other);
  SharedPtr(SharedPtr&& other) noexcept;
  SharedPtr& operator=(SharedPtr&& other) noexcept;
  void Reset(T* ptr = nullptr);
  void Swap(SharedPtr& other);
  T* Get() const;
  size_t UseCount() const;
  T& operator*() const;
  T* operator->() const;
  explicit operator bool() const;
  ~SharedPtr();

  friend class WeakPtr<T>;
};

template <class T>
SharedPtr<T>::SharedPtr() : shared_ptr_(nullptr), counter_(nullptr) {
}

template <class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other) : shared_ptr_(other.shared_ptr_), counter_(other.counter_) {
  if (other.counter_ != nullptr) {
    ++(other.counter_->strong_counter);
  }
}

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& weak_ptr) {
  if (!weak_ptr.Expired()) {
    shared_ptr_ = weak_ptr.weak_ptr_;
    counter_ = weak_ptr.counter_;
    ++(counter_->strong_counter);
  } else {
    throw BadWeakPtr();
  }
}

template <class T>
SharedPtr<T>::SharedPtr(T* ptr) : shared_ptr_(ptr), counter_(nullptr) {
  if (ptr != nullptr) {
    counter_ = new Counter(1, 0);
  }
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
  SharedPtr<T> copy(other);
  Swap(copy);
  return *this;
}

template <class T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other) noexcept : shared_ptr_(other.shared_ptr_), counter_(other.counter_) {
  other.shared_ptr_ = nullptr;
  other.counter_ = nullptr;
}

template <class T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other) noexcept {
  SharedPtr<T> copy(std::move(other));
  Swap(copy);
  return *this;
}

template <class T>
void SharedPtr<T>::Reset(T* ptr) {
  SharedPtr<T> copy(std::move(*this));
  shared_ptr_ = ptr;
  if (ptr != nullptr) {
    counter_ = new Counter(1, 0);
  } else {
    counter_ = nullptr;
  }
}

template <class T>
void SharedPtr<T>::Swap(SharedPtr<T>& other) {
  std::swap(counter_, other.counter_);
  std::swap(shared_ptr_, other.shared_ptr_);
}

template <class T>
T* SharedPtr<T>::Get() const {
  return shared_ptr_;
}

template <class T>
size_t SharedPtr<T>::UseCount() const {
  if (counter_ == nullptr) {
    return 0;
  }
  return counter_->strong_counter;
}

template <class T>
T& SharedPtr<T>::operator*() const {
  return *shared_ptr_;
}

template <class T>
T* SharedPtr<T>::operator->() const {
  return shared_ptr_;
}

template <class T>
SharedPtr<T>::operator bool() const {
  return (shared_ptr_ != nullptr);
}

template <class T>
SharedPtr<T>::~SharedPtr() {
  if (counter_ != nullptr) {
    // --(counter_->strong_counter);
    if (counter_->strong_counter - 1 == 0) {
      delete shared_ptr_;
      if (counter_->weak_counter == 0) {
        delete counter_;
      } else {
        --(counter_->strong_counter);
      }
    } else {
      --(counter_->strong_counter);
    }
    counter_ = nullptr;
    shared_ptr_ = nullptr;
  }
}

template <class T>
class WeakPtr {
  Counter* counter_;
  T* weak_ptr_;

 public:
  WeakPtr();
  WeakPtr(const WeakPtr& other);  // NOLINT
  WeakPtr(WeakPtr&& other) noexcept;
  WeakPtr& operator=(const WeakPtr& other);
  WeakPtr& operator=(WeakPtr&& other) noexcept;
  WeakPtr(const SharedPtr<T>& shared_ptr);  // NOLINT
  void Swap(WeakPtr& other);
  void Reset();
  size_t UseCount() const;
  bool Expired() const;
  SharedPtr<T> Lock() const;
  ~WeakPtr();

  friend class SharedPtr<T>;
};

template <class T>
WeakPtr<T>::WeakPtr() : counter_(nullptr), weak_ptr_(nullptr) {
}

template <class T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other) : counter_(other.counter_), weak_ptr_(other.weak_ptr_) {
  if (other.counter_ != nullptr) {
    ++(other.counter_->weak_counter);
  }
}

template <class T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& other) noexcept : counter_(other.counter_), weak_ptr_(other.weak_ptr_) {
  other.weak_ptr_ = nullptr;
  other.counter_ = nullptr;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other) {
  WeakPtr<T> copy(other);
  Swap(copy);
  return *this;
}

template <class T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other) noexcept {
  WeakPtr<T> copy(std::move(other));
  Swap(copy);
  return *this;
}

template <class T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared_ptr) : counter_(shared_ptr.counter_), weak_ptr_(shared_ptr.shared_ptr_) {
  if (counter_ != nullptr) {
    ++(counter_->weak_counter);
  }
}

template <class T>
void WeakPtr<T>::Swap(WeakPtr<T>& other) {
  std::swap(counter_, other.counter_);
  std::swap(weak_ptr_, other.weak_ptr_);
}

template <class T>
void WeakPtr<T>::Reset() {
  WeakPtr<T> copy(std::move(*this));
}

template <class T>
size_t WeakPtr<T>::UseCount() const {
  if (counter_ == nullptr) {
    return 0;
  }
  return counter_->strong_counter;
}

template <class T>
bool WeakPtr<T>::Expired() const {
  return (UseCount() == 0);
}

template <class T>
SharedPtr<T> WeakPtr<T>::Lock() const {
  if (Expired()) {
    return nullptr;
  }
  return SharedPtr<T>(*this);
}

template <class T>
WeakPtr<T>::~WeakPtr() {
  if (counter_ != nullptr) {
    --(counter_->weak_counter);
    if ((counter_->strong_counter == 0) && (counter_->weak_counter == 0)) {
      delete counter_;
    }
    counter_ = nullptr;
    weak_ptr_ = nullptr;
  }
}

#endif  // SHARED_PTR_
