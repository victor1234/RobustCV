#pragma once

#include <vector>

namespace rcv {
template <typename T> class Matrix {
  public:
	Matrix();

	unsigned int rows() const;
	unsigned int columns() const;
	unsigned int channels() const;

  private:
	std::vector<T> data;
	unsigned int _rows;
	unsigned int _columns;
	unsigned int _channels;
};
} // namespace rcv

namespace rcv {
template <typename T> Matrix<T>::Matrix() : _rows(0), _columns(0), _channels(0) {}

template <typename T> unsigned int Matrix<T>::rows() const { return _rows; }

template <typename T> unsigned int Matrix<T>::columns() const { return _columns; }

template <typename T> unsigned int Matrix<T>::channels() const { return _channels; }
} // namespace rcv
