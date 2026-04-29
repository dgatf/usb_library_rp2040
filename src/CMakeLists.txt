add_library(${PROJECT_NAME} 
    usb.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    hardware_irq
)

