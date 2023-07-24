#pragma once

#include <vector>

namespace rcv {
template <typename T> class Matrix {
  public:
	Matrix();
	Matrix(unsigned int rows, unsigned int columns);
	Matrix(unsigned int rows, unsigned int columns, unsigned int channels);

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

template <typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int columns) : _rows(rows), _columns(columns), _channels(1)
{
	data.resize(_rows * _columns * _channels);
}

template <typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int columns, unsigned int channels)
	: _rows(rows), _columns(columns), _channels(channels)
{
	data.resize(_rows * _columns * _channels);
}

template <typename T> unsigned int Matrix<T>::rows() const { return _rows; }

template <typename T> unsigned int Matrix<T>::columns() const { return _columns; }

template <typename T> unsigned int Matrix<T>::channels() const { return _channels; }
} // namespace rcv
