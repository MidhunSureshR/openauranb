cmake_minimum_required(VERSION 3.13)
project(OpenauraNB)
set(CMAKE_C_STANDARD 99)


add_executable(openauranb openauranb.c backlight.c backlight.h error.c error.h device.c device.h input.c input.h)
target_link_libraries(openauranb usb-1.0)