cmake_minimum_required(VERSION 3.16)
project(polygot_consumer C)

set(CMAKE_C_STANDARD 11)

find_package(polygot_parser REQUIRED CONFIG)

add_executable(consumer
  main.c
)

target_link_libraries(consumer PRIVATE polygot::parser)
