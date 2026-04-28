cmake_minimum_required(VERSION 3.12)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(usb_library C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(src)
add_subdirectory(examples/usb_device_example)
