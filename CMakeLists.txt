cmake_minimum_required(VERSION 3.21)
project(CartesianProducts)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt)

add_executable(CartesianProducts main.cpp)
target_compile_options(CartesianProducts PUBLIC -Wall -Werror -fsanitize=address)
target_link_options(CartesianProducts PUBLIC -fsanitize=address)
target_link_libraries(CartesianProducts fmt::fmt)