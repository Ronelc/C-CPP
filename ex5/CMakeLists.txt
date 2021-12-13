cmake_minimum_required(VERSION 3.19)
project(ex5)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex5 main.cpp Matrix.cpp Activation.cpp Dense.cpp MlpNetwork.cpp)