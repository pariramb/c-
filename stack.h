#ifndef STACK_
#define STACK_
#include <deque>
#include <memory>
#include <iterator>

template <typename T, typename Container = std::deque<T>>
class Stack {
 private:
  Container cont_;

 public:
  Stack() = default;
  explicit Stack(const Container&);
  template <typename Iterator>
  Stack(Iterator, Iterator);
  bool Empty() const;
  size_t Size() const;
  template <typename U = T>
  void Push(U&& value);
  template <typename... Args>
  void Emplace(Args&&... args);
  void Pop();
  void Swap(Stack&);
  T& Top();
  const T& Top() const;
};

template <typename T, typename Container>
Stack<T, Container>::Stack(const Container& cont) : cont_(cont) {
}

template <typename T, typename Container>
template <typename Iterator>
Stack<T, Container>::Stack(Iterator begin, Iterator end) : cont_(begin, end) {
}

template <typename T, typename Container>
const T& Stack<T, Container>::Top() const {
  return cont_.back();
}

template <typename T, typename Container>
T& Stack<T, Container>::Top() {
  return cont_.back();
}

template <typename T, typename Container>
bool Stack<T, Container>::Empty() const {
  return cont_.empty();
}

template <typename T, typename Container>
size_t Stack<T, Container>::Size() const {
  return cont_.size();
}

template <typename T, typename Container>
template <typename U>
void Stack<T, Container>::Push(U&& value) {
  cont_.emplace_back(std::forward<U>(value));
}

template <typename T, typename Container>
void Stack<T, Container>::Pop() {
  cont_.pop_back();
}

template <typename T, typename Container>
void Stack<T, Container>::Swap(Stack& stack) {
  std::swap(*this, stack);
}

template <typename T, typename Container>
template <typename... Args>
void Stack<T, Container>::Emplace(Args&&... args) {
  cont_.emplace_back(std::forward<Args>(args)...);
}

#endif
