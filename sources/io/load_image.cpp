#include "load_jpeg.hpp"

#include <RobustCV/io/load_image.hpp>

namespace rcv::io {

Matrix<uint8_t> loadImage(const std::string filename) { return loadJpeg(filename); }
} // namespace rcv::io
