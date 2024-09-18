cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(usb_device C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME} 
    main.c
    usb.c
    usb_config.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_irq
)
 
