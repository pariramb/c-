#ifndef SHABLON_H_
#define SHABLON_H_

template <class T, class U>

bool BinarySearch(const T begin, const T end, const U& value) {
  T mid;
  T left = begin - 1;
  T right = end;
  while (1 < (right - left)) {
    mid = left + (right - left) / 2;
    if (*mid < value) {
      left = mid;
    } else {
      right = mid;
    }
  }
  return (((right != end) && !((*right < value) || (value < *right))) ? true : false);
}

template <class T, class U>

T LowerBound(const T begin, const T end, const U& value) {
  T mid;
  T left = begin - 1;
  T right = end;
  while (1 < (right - left)) {
    mid = left + (right - left) / 2;
    if (*mid < value) {
      left = mid;
    } else {
      right = mid;
    }
  }
  return right;
}

template <class T, class U>
T UpperBound(const T begin, const T end, const U& value) {
  T mid;
  T left = begin - 1;
  T right = end;
  while (1 < (right - left)) {
    mid = left + (right - left) / 2;
    if ((*mid < value)  (value < *mid))) {
      left = mid;
    } else {
      right = mid;
    }
  }
  return right;
}

#endif  // SHABLON_H_