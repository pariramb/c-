
#ifndef ENUMERATE_
#define ENUMERATE_

#include <utility>
#include <iterator>

template <typename T>
class Enumerate {
  T& container_;

 public:
  using ValueType = typename T::value_type;
  using IteratorType = typename T::iterator;
  using ConstIteratorType = typename T::const_iterator;

  explicit Enumerate(T& container) : container_(container) {
  }

  class Iterator {
   public:
    IteratorType it;
    int counter;

    Iterator(IteratorType&& it, int&& counter) : it(it), counter(counter) {
    }

    bool operator!=(const Iterator& second) const {
      return it != second.it;
    }

    void operator++() {
      ++it;
      ++counter;
    }

    auto operator*() const {
      return std::pair<int, ValueType&>(counter, *it);
    }
  };

  class ConstIterator {
   public:
    using Base = std::pair<ConstIteratorType, int>;
    ConstIteratorType it;
    int counter;
    ConstIterator(ConstIteratorType it, int index) : it(it), counter(counter) {
    }

    bool operator!=(ConstIteratorType& second) const {
      return it != second.it;
    }

    void operator++() {
      ++it;
      ++counter;
    }

    auto operator*() const {
      return std::pair<int, ValueType&>(counter, *it);
    }
  };

  Iterator begin() {  //  NOLINT
    return Iterator(container_.begin(), 0);
  }

  Iterator end() {  //  NOLINT
    return Iterator(container_.end(), std::distance(container_.begin(), container_.end()));
  }

  ConstIterator begin() const {  //  NOLINT
    return ConstIterator(container_.cbegin(), 0);
  }

  ConstIterator end() const {  //  NOLINT
    return ConstIterator(container_.cend(), std::distance(container_.cbegin(), container_.cend()));
  }
};

template <typename T>
class Enumerate<const T> {
  const T& container_;

 public:
  using ValueType = typename T::value_type;
  using ConstIteratorType = typename T::const_iterator;

  explicit Enumerate(const T& container) : container_(container) {
  }

  class ConstIterator {
   public:
    ConstIteratorType it;
    int counter;

    ConstIterator(ConstIteratorType it, int counter) : it(it), counter(counter) {
    }

    bool operator!=(ConstIterator& second) const {
      return it != second.it;
    }

    void operator++() {
      ++it;
      ++counter;
    }

    auto operator*() const {
      return std::pair<int, const ValueType&>(counter, *it);
    }
  };

  ConstIterator begin() const {  //  NOLINT
    return ConstIterator(container_.cbegin(), 0);
  }

  ConstIterator end() const {  //  NOLINT
    return ConstIterator(container_.cend(), std::distance(container_.cbegin(), container_.cend()));
  }
};
#endif
