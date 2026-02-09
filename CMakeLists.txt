cmake_minimum_required(VERSION 3.27)
project(OCR)

set(CMAKE_CXX_STANDARD 17)

add_executable(OCR main.cpp
        matrix.h
        matrix.cpp)
