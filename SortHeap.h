#ifndef C_H_
#define C_H_
template <class T>
void Swap(T& a, T& b) {
  T a_temp = a;
  a = b;
  b = a_temp;
}

template <class T>
T* Parent(T* child, T* begin) {
  if (begin < child) {
    return begin + ((child - begin) - 1) / 2;
  }
  return nullptr;
}

template <class T>
T* LeftChild(T* parent, T* end, T* begin) {
  T* left_child = parent + (parent - begin) + 1;
  if (left_child < end) {
    return left_child;
  }
  return nullptr;
}

template <class T>
T* RightChild(T* parent, T* end, T* begin) {
  T* right_child = parent + (parent - begin) + 2;
  if (right_child < end) {
    return right_child;
  }
  return nullptr;
}

template <class T>
void SiftDown(T* begin, T* end, T* begin_temp) {
  T* left = LeftChild(begin_temp, end, begin);
  T* right = RightChild(begin_temp, end, begin);
  T* largest = begin_temp;
  if ((left != nullptr) && (*largest < *left)) {
    largest = left;
  }
  if ((right != nullptr) && (*largest < *right)) {
    largest = right;
  }
  if (largest != begin_temp) {
    Swap(*largest, *begin_temp);
    SiftDown(begin, end, largest);
  }
}

template <class T>
void MakeHeap(T* begin, T* end) {
  for (int i = ((end - begin) - 1) / 2; i > -1; --i) {
    SiftDown(begin, end, begin + i);
  }
}

template <class T>
void SortHeap(T* begin, T* end) {
  MakeHeap(begin, end);
  for (int i = end - begin - 1; i > 0; --i) {
    Swap(begin[i], begin[0]);
    T* begin_temp = begin;

    SiftDown(begin, begin + i, begin_temp);
  }
}
#endif