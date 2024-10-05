cmake_minimum_required(VERSION 3.29)
project(QR_Generator)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(QR_Generator ${OpenCV_LIBS})

include_directories(include)

add_executable(QR_Generator
        src/main.cpp
        src/utils/qr_encoder.cpp
        src/utils/reed_solomon.cpp
        src/utils/qr_matrix.cpp
)
