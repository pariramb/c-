#ifndef UNIQUE_PTR_
#define UNIQUE_PTR_

template <class T>
class UniquePtr {

 private:
  T* unique_ptr_;

 public:
  UniquePtr() : unique_ptr_(nullptr) {
  }

  UniquePtr(T* object) : unique_ptr_(object) {  // NOLINT
  }

  UniquePtr(UniquePtr<T>&& object) noexcept {
    unique_ptr_ = object.unique_ptr_;
    object.unique_ptr_ = nullptr;
  }

  UniquePtr<T>& operator=(const UniquePtr<T>& object) = delete;

  UniquePtr(const UniquePtr<T>& object) = delete;

  UniquePtr<T>& operator=(UniquePtr<T>&& object) noexcept {
    if (this != &object) {
      delete unique_ptr_;
      unique_ptr_ = object.unique_ptr_;
      object.unique_ptr_ = nullptr;
    }
    return *this;
  }

  T* Release() {
    T* temp_ptr = unique_ptr_;
    unique_ptr_ = nullptr;
    return temp_ptr;
  }

  void Reset(T* ptr = nullptr) {
    delete unique_ptr_;
    unique_ptr_ = ptr;
  }

  void Swap(UniquePtr<T>& object) {
    T* temp = unique_ptr_;
    unique_ptr_ = object.unique_ptr_;
    object.unique_ptr_ = temp;
  }

  T* Get() const {
    return unique_ptr_;
  }

  T& operator*() const {
    return *unique_ptr_;
  }

  T* operator->() const {
    return unique_ptr_;
  }

  explicit operator bool() const {
    return unique_ptr_;
  }

  ~UniquePtr() {
    delete unique_ptr_;
  }
};
#endif  // UNIQUE_PTR_