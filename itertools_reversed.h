#ifndef REVERSED_H_
#define REVERSED_H_
#define REVERSE_REVERSED_IMPLEMENTED
#define TEMPORARY_REVERSED_IMPLEMENTED

#include <type_traits>

template <typename T>
class ReversedObject {
 private:
  T obj_;

 public:
  explicit ReversedObject(T obj) : obj_(obj) {
  }

  decltype(auto) rbegin() const;  // NOLINT

  decltype(auto) rend() const;  //  NOLINT

  decltype(auto) begin() const;  // NOLINT

  decltype(auto) end() const;  //  NOLINT
};

template <typename T>
decltype(auto) ReversedObject<T>::rbegin() const {  // NOLINT
  return obj_.begin();
}

template <typename T>
decltype(auto) ReversedObject<T>::rend() const {  // NOLINT
  return obj_.end();
}

template <typename T>
decltype(auto) ReversedObject<T>::begin() const {  // NOLINT
  return obj_.rbegin();
}

template <typename T>
decltype(auto) ReversedObject<T>::end() const {  // NOLINT
  return obj_.rend();
}

template <typename T>
decltype(auto) Reversed(const T& obj) {
  return ReversedObject<T>(obj);
}

template <typename T>
decltype(auto) Reversed(T&& obj) {
  return ReversedObject<T>(std::forward<T>(obj));
}

#endif