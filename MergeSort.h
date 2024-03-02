#ifndef MERGE_H_
#define MERGE_H_

template <class T, class U, class W>
W* Merge(const T* first_begin, const T* first_end, const U* second_begin, const U* second_end, W* out) {
  const T* first_temp = first_begin;
  const U* second_temp = second_begin;
  while ((first_temp < first_end) && (second_temp < second_end)) {
    if (*second_temp < *first_temp) {
      *out++ = *second_temp++;
    } else {
      *out++ = *first_temp++;
    }
  }
  while (first_temp < first_end) {
    *out++ = *first_temp++;
  }
  while (second_temp < second_end) {
    *out++ = *second_temp++;
  }
  return out;
}
#endif  // MERGE_H_