if(NOT TARGET pico_stdlib OR NOT TARGET hardware_irq)
    message(FATAL_ERROR "usb_library_rp2040 requires the Pico SDK to be imported and initialized before adding this directory.")
endif()

add_library(usb_library_rp2040
    usb.c
)

add_library(usb_library_rp2040::usb_library_rp2040 ALIAS usb_library_rp2040)

target_include_directories(usb_library_rp2040 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(usb_library_rp2040 PUBLIC
    pico_stdlib
    hardware_irq
)
