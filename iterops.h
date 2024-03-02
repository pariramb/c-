#ifndef ITEROPS_
#define ITEROPS_

#include <iostream>
#include <type_traits>

template <class Iter>
constexpr void Advance(Iter& iter, const int& steps) {
  using Tag = typename std::iterator_traits<Iter>::iterator_category;
  if constexpr (std::is_same<Tag, std::random_access_iterator_tag>::value ||
                std::is_base_of<std::random_access_iterator_tag, Tag>::value) {
    iter += steps;
  } else {
    if constexpr (std::is_same<Tag, std::bidirectional_iterator_tag>::value ||
                  std::is_base_of<std::bidirectional_iterator_tag, Tag>::value) {
      if (steps >= 0) {
        for (int i = 0; i < steps; ++i) {
          ++iter;
        }
      } else {
        for (int i = 0; i < -steps; ++i) {
          --iter;
        }
      }
    } else {
      for (int i = 0; i < steps; ++i) {
        ++iter;
      }
    }
  }
}

template <class Iter>
constexpr Iter Next(const Iter& iter, const int& steps = 1) {
  auto iter_temp = iter;
  Advance(iter_temp, steps);
  return iter_temp;
}

template <class Iter>
constexpr Iter Prev(const Iter& iter, const int& steps = 1) {
  return Next(iter, -steps);
}

template <class Iter>
constexpr decltype(auto) Distance(const Iter& begin, const Iter& end) {
  using Tag = typename std::iterator_traits<Iter>::iterator_category;
  if constexpr (std::is_same<Tag, std::random_access_iterator_tag>::value ||
                std::is_base_of<std::random_access_iterator_tag, Tag>::value) {
    return end - begin;
  } else {
    typename std::iterator_traits<Iter>::difference_type distance{};
    for (auto iter_temp = begin; iter_temp != end; ++iter_temp) {
      ++distance;
    }
    return distance;
  }
}

#endif
