add_library(usb_library
    usb.c
)

target_include_directories(usb_library PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(usb_library PUBLIC
    pico_stdlib
    hardware_irq
)