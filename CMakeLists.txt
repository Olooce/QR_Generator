cmake_minimum_required(VERSION 3.29)
project(QR_Generator)

set(CMAKE_CXX_STANDARD 20)

# Find OpenCV and include its directories
find_package(OpenCV REQUIRED)

# Add the executable target
add_executable(QR_Generator
        src/main.cpp
        src/utils/qr_encoder.cpp
        src/utils/reed_solomon.cpp
        src/utils/qr_matrix.cpp
)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

# Link OpenCV to the target
target_link_libraries(QR_Generator ${OpenCV_LIBS})
