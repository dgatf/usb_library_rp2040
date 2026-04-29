add_executable(usb_device_example
    main.c
    usb_config.c
)

target_include_directories(usb_device_example PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(usb_device_example
    PRIVATE
        usb_library
        pico_stdlib
)

pico_enable_stdio_usb(usb_device_example 0)
pico_enable_stdio_uart(usb_device_example 1)

pico_add_extra_outputs(usb_device_example)