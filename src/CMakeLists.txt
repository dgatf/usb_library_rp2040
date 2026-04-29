add_library(usb_library_rp2040
    usb.c
)

target_include_directories(usb_library_rp2040 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(usb_library_rp2040 PUBLIC
    pico_stdlib
    hardware_irq
)