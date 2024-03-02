#ifndef ANY_
#define ANY_

#include <stdexcept>
#include <memory>
#include <type_traits>

template <typename T>
using RemoveCvrefT = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

class BadAnyCast : public std::bad_cast {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "BadAnyCast error";
  }
};

class Any {
  struct IHolder {
    virtual std::shared_ptr<IHolder> Clone() = 0;
    virtual ~IHolder() = 0;
  };

  template <typename T>
  struct AnyHolder : IHolder {
    T value_;

    std::shared_ptr<IHolder> Clone() override {
      return std::make_shared<AnyHolder<T>>(value_);
    }
    explicit AnyHolder(T&& value) noexcept : value_(std::move(value)) {
    }
    explicit AnyHolder(const T& value) : value_(value) {
    }
    ~AnyHolder() override = default;
    template <typename U>
    friend U AnyCast(const Any&);  // NOLINT
  };
  std::shared_ptr<IHolder> ptr_;

 public:
  Any() = default;
  Any(const Any& other) : ptr_(other.ptr_->Clone()) {
  }
  Any(Any&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }
  template <typename U, typename = std::enable_if_t<!std::is_same_v<std::decay_t<U>, Any>>>  // delay
  Any(U&& value) {                                                                           // NOLINT
    ptr_ = std::make_shared<AnyHolder<RemoveCvrefT<U>>>(std::forward<U>(value));
  }
  Any& operator=(const Any& other) {
    Any temp(other);
    Swap(temp);
    return *this;
  }
  Any& operator=(Any&& other) noexcept {
    Any temp(std::move(other));
    Swap(temp);
    return *this;
  }

  template <typename U>
  Any& operator=(U&& value) {
    Any temp(std::forward<U>(value));
    Swap(temp);
    return *this;
  }
  void Swap(Any& other) {
    ptr_.swap(other.ptr_);
  }
  void Reset() {
    ptr_.reset();
  }
  bool HasValue() const {
    return (ptr_ != nullptr);
  }
  ~Any() = default;
  template <typename T>
  friend T AnyCast(const Any&);  // NOLINT
};
inline Any::IHolder::~IHolder() = default;

template <typename T>
T AnyCast(const Any& obj) {  // NOLINT
  if (!obj.HasValue()) {
    throw BadAnyCast();
  }
  auto obj_ptr = std::dynamic_pointer_cast<Any::AnyHolder<T>>(obj.ptr_);
  if (obj_ptr == nullptr) {
    throw BadAnyCast();
  }
  return obj_ptr->value_;
}
#endif