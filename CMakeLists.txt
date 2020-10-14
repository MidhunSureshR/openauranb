cmake_minimum_required(VERSION 3.13)
project(OpenauraNB)
set(CMAKE_C_STANDARD 99)


add_executable(openauranb src/openauranb.c src/backlight.c src/backlight.h src/error.c src/error.h src/device.c src/device.h src/input.c src/input.h)
target_link_libraries(openauranb usb-1.0)