cmake_minimum_required(VERSION 3.13)
project(OpenauraNB)
set(CMAKE_C_STANDARD 99)


add_executable(openauranb rewrite.c backlight.c backlight.h)
target_link_libraries(openauranb usb-1.0)