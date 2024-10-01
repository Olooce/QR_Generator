cmake_minimum_required(VERSION 3.29)
project(QR_Generator)

set(CMAKE_CXX_STANDARD 20)


include_directories(include)

add_executable(QR_Generator
        src/main.cpp
        src/utils/qr_encoder.cpp
        src/include/qr_encoder.h
        src/include/reed_solomon.h
        src/include/qr_matrix.h
        src/utils/reed_solomon.cpp
        src/utils/qr_matrix.cpp
)
