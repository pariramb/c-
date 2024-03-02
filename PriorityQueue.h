#ifndef S_
#define S_
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
void PushHeap(T* begin, T* end) {
  T* parent = Parent(end - 1, begin);
  if ((parent != nullptr) && (*parent < *(end - 1))) {
    Swap(*parent, *(end - 1));
    PushHeap(begin, parent + 1);
  }
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
void PopHeap(T* begin, T* end) {
  T* begin_temp = begin;
  Swap(*begin, *(end - 1));
  SiftDown(begin, (end - 1), begin_temp);
}
#endif