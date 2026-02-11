cmake_minimum_required(VERSION 3.10)
project(OCR)

set(CMAKE_CXX_STANDARD 17)


include_directories(/opt/homebrew/include)
include_directories(/usr/local/include)
link_directories(/opt/homebrew/lib)
link_directories(/usr/local/lib)

# Trouver les paquets nécessaires
find_package(OpenGL REQUIRED)
find_library(GLFW_LIB glfw)

add_executable(OCR main.cpp matrix.cpp matrix.h)

if (APPLE)
    target_link_libraries(OCR ${GLFW_LIB} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
else()
    target_link_libraries(OCR glfw OpenGL::GL)
endif()