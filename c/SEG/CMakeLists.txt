cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(SEG_project)

# Initialize the SDK
pico_sdk_init()

add_executable(SEG
        SEG.c
)
            
# pull in common dependencies
target_link_libraries(SEG hardware_spi hardware_pwm pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(SEG 1)
pico_enable_stdio_uart(SEG 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(SEG)

