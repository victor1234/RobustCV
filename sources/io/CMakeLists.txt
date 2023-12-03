add_library(io load_jpeg.cpp load_image.cpp)

target_link_libraries(io rcv_meta)
target_link_libraries(${RCV_TARGET_NAME} INTERFACE io)
