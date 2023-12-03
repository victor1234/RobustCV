#pragma once

#include <RobustCV/Matrix.hpp>

#include <string>

namespace rcv::io {

Matrix<uint8_t> loadImage(const std::string filename);

} // namespace rcv::io
