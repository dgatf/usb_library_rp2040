cmake_minimum_required(VERSION 3.12)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(USB_LIBRARY_RP2040_TOP_LEVEL ON)
else()
    set(USB_LIBRARY_RP2040_TOP_LEVEL OFF)
endif()

if(USB_LIBRARY_RP2040_TOP_LEVEL)
    include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

    project(usb_library C CXX ASM)

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)

    pico_sdk_init()
endif()

option(USB_LIBRARY_RP2040_BUILD_EXAMPLES "Build usb_library examples" ${USB_LIBRARY_RP2040_TOP_LEVEL})

add_subdirectory(src)

if(USB_LIBRARY_RP2040_BUILD_EXAMPLES)
    add_subdirectory(examples/usb_device_example)
endif()
