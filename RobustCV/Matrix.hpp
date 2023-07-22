#pragma once

#include <vector>

namespace rcv {
template <typename T> class Matrix {
  public:
	Matrix();

	int rows() const;
	int columns() const;

  private:
	std::vector<T> data;
	int _rows;
	int _columns;
};
} // namespace rcv

namespace rcv {
template <typename T> Matrix<T>::Matrix() : _rows(0), _columns(0) {}

template <typename T> int Matrix<T>::rows() const { return _rows; }

template <typename T> int Matrix<T>::columns() const { return _columns; }
} // namespace rcv
