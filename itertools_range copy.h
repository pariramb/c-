#ifndef RANGE_
#define RANGE_

#include <iterator>

class Range {
 private:
  int begin_;
  int step_;
  int end_;

 public:
  template <bool IsConst>
  class Iter;
  using Iterator = Iter<false>;
  using ConstIterator = Iter<true>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;
  Range(int begin, int end, int step = 1) : begin_(begin), step_(step), end_(end) {
    if (!step_) {
      end_ = begin_;
    }
  }
  explicit Range(int end) : begin_(0), step_(1), end_(end) {
    if (end_ <= 0) {
      end_ = 0;
    }
  }

  Iterator begin();  // NOLINT

  ConstIterator begin() const;  // NOLINT

  Iterator end();  // NOLINT

  ConstIterator end() const;  //  NOLINT

  Iterator rbegin();  // NOLINT

  ConstIterator rbegin() const;  // NOLINT

  Iterator rend();  // NOLINT

  ConstIterator rend() const;  //  NOLINT
  template <bool IsConst>
  class Iter {
   private:
    using Type = std::conditional_t<IsConst, const int, int>;
    int element_;
    int step_;

   public:
    Type operator*() {
      return element_;
    }
    Iter& operator++() {
      element_ += step_;
      return *this;
    }
    template <bool IsConstNew>
    bool operator!=(const Iter<IsConstNew>&);

    explicit Iter(int element, int step) : element_(element), step_(step) {
    }
    using reference = Type&;                                    // NOLINT
    using pointer = Type*;                                      // NOLINT
    using iterator_category = std::bidirectional_iterator_tag;  // NOLINT
    using value_type = int;                                     // NOLINT
    using difference_type = int;                                // NOLINT
  };
};

typename Range::Iterator Range::begin() {  // NOLINT
  return Range::Iterator(begin_, step_);
}

typename Range::ConstIterator Range::begin() const {  // NOLINT
  return Range::ConstIterator(begin_, step_);
}

typename Range::Iterator Range::end() {  // NOLINT
  return Range::Iterator(end_, step_);
}

typename Range::ConstIterator Range::end() const {  // NOLINT
  return Range::ConstIterator(end_, step_);
}

typename Range::Iterator Range::rbegin() {  // NOLINT
  return Range::Iterator(end_, step_);
}

typename Range::ConstIterator Range::rbegin() const {  // NOLINT
  return Range::ConstIterator(end_, step_);
}

typename Range::Iterator Range::rend() {  // NOLINT
  return Range::Iterator(begin_, step_);
}

typename Range::ConstIterator Range::rend() const {  // NOLINT
  return Range::ConstIterator(begin_, step_);
}

template <bool IsConst>
template <bool IsConstNew>
bool Range::Iter<IsConst>::operator!=(const Range::Iter<IsConstNew>& it) {
  if (step_ < 0) {
    return (element_ > it.element_);
  }
  return (element_ < it.element_);
}

#endif
