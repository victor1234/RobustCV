#pragma once

#include <RobustCV/Matrix.hpp>
#include <bits/stdint-uintn.h>
#include <string>

namespace rcv::io {
Matrix<uint8_t> loadJpeg(const std::string filename);
}
