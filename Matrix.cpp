#ifndef matrixH_
#define matrixH_

#include <iostream>
#include <stdexcept>
class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {
  }
};

template <class T, size_t N, size_t M>
struct Matrix {
  T matrix[N][M];
  size_t RowsNumber() const {
    return N;
  }
  size_t ColumnsNumber() const {
    return M;
  }
  T& operator()(const size_t& i, const size_t& j) {
    return matrix[i][j];
  }
  const T& operator()(const size_t& i, const size_t& j) const {
    return matrix[i][j];
  }
  T& At(const size_t& i, const size_t& j) {
    if ((i >= N) || (j >= M)) {
      throw MatrixOutOfRange{};
    }
    return matrix[i][j];
  }
  const T& At(const size_t& i, const size_t& j) const {
    if ((i >= N) || (j >= M)) {
      throw MatrixOutOfRange{};
    }
    return matrix[i][j];
  }
  Matrix<T, N, M>& operator+=(const Matrix<T, N, M>& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix[i][j] += other.matrix[i][j];
      }
    }
    return *this;
  }

  Matrix<T, N, M>& operator-=(const Matrix<T, N, M>& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix[i][j] -= other.matrix[i][j];
      }
    }
    return *this;
  }

  Matrix<T, N, M>& operator*=(const Matrix<T, M, M>& other) {
    *this = *this * other;
    return *this;
  }
  Matrix<T, N, M>& operator*=(const int64_t& value) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix[i][j] *= value;
      }
    }
    return *this;
  }
  bool operator==(const Matrix<T, N, M>& other) const {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        if (matrix[i][j] != other.matrix[i][j]) {
          return false;
        }
      }
    }
    return true;
  }
  bool operator!=(const Matrix<T, N, M>& other) const {
    return !(*this == other);
  }
  Matrix<T, N, M>& operator/=(const int64_t& value);
};
template <class T, size_t N, size_t M>
Matrix<T, N, M>& Matrix<T, N, M>::operator/=(const int64_t& value) {
  if (value != 0) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrix[i][j] /= value;
      }
    }
  }
  return *this;
}
template <class T, size_t N, size_t M>
Matrix<T, M, N> GetTransposed(const Matrix<T, N, M>& matrix) {
  Matrix<T, M, N> transponed;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      transponed.matrix[j][i] = matrix.matrix[i][j];
    }
  }
  return transponed;
}
template <class T, size_t N, size_t M>
Matrix<T, N, M> operator+(const Matrix<T, N, M>& x, const Matrix<T, N, M>& y) {
  Matrix<T, N, M> result = x;
  result += y;
  return result;
}

template <class T, size_t N, size_t M>
Matrix<T, N, M> operator-(const Matrix<T, N, M>& x, const Matrix<T, N, M>& y) {
  Matrix<T, N, M> result = x;
  result -= y;
  return result;
}

template <class T, size_t N, size_t M, size_t S>
Matrix<T, N, S> operator*(const Matrix<T, N, M>& x, const Matrix<T, M, S>& y) {
  Matrix<T, N, S> result{};
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < S; ++j) {
      for (size_t k = 0; k < M; ++k) {
        result.matrix[i][j] += x.matrix[i][k] * y.matrix[k][j];
      }
    }
  }
  return result;
}

template <class T, size_t N, size_t M>
Matrix<T, N, M> operator*(const Matrix<T, N, M>& x, const int64_t value) {
  Matrix<T, N, M> result = x;
  result *= value;
  return result;
}

template <class T, size_t N, size_t M>
Matrix<T, N, M> operator*(const int64_t& value, const Matrix<T, N, M>& x) {
  Matrix<T, N, M> result = x;
  result *= value;
  return result;
}

template <class T, size_t N, size_t M>
Matrix<T, N, M> operator/(const Matrix<T, N, M>& x, const int64_t& value) {
  Matrix<T, N, M> result = x;
  result /= value;
  return result;
}

template <class T, size_t N, size_t M>
std::ostream& operator<<(std::ostream& os, const Matrix<T, N, M> value) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M - 1; ++j) {
      os << value.matrix[i][j] << ' ';
    }
    os << value.matrix[i][M - 1] << '\n';
  }
  return os;
}

template <class T, size_t N, size_t M>
std::istream& operator>>(std::istream& is, Matrix<T, N, M>& value) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      is >> value.matrix[i][j];
    }
  }
  return is;
}
#endif