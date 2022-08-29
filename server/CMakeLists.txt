cmake_minimum_required(VERSION 3.24)
project(smart_led
        VERSION 0.0.1
        DESCRIPTION "Raspberry PI server to control arduino led strip"
        HOMEPAGE_URL "https://github.com/danpashin/smart_led_strip"
        LANGUAGES CXX C
        )

find_program(FPM_EXECUTABLE NAMES fpm)
if (NOT FPM_EXECUTABLE)
    message(FATAL_ERROR "fpm is required for build. Please check you have it installed with 'gem install fpm'")
endif ()

set(PROJECT_AUTHOR danpashin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -Wpedantic)

# Hardcoded as project is only available for rpi
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_SYSROOT /usr/aarch64-linux-gnu)

include(cmake/libp2c.cmake)
include(cmake/liboping.cmake)

add_executable(smart_led
        src/main.cpp
        src/arduino.cpp
        src/color.cpp
        src/day_part.cpp
        src/arp.cpp)
target_include_directories(smart_led PRIVATE include)
target_link_libraries(smart_led pi2c oping)

set(FPM_ARGS
        -f
        --input-type dir
        --output-type pacman
        --log error
        --name ${PROJECT_NAME}
        --maintainer ${PROJECT_AUTHOR}
        --description "${PROJECT_DESCRIPTION}"
        --version ${PROJECT_VERSION}
        --url ${PROJECT_HOMEPAGE_URL}
        --architecture aarch64
        --license GPL3
        --after-install ${PROJECT_SOURCE_DIR}/assets/reload-systemd.sh
        --after-remove ${PROJECT_SOURCE_DIR}/assets/reload-systemd.sh

        $<TARGET_FILE:smart_led>=/usr/bin/smart_led
        ${PROJECT_SOURCE_DIR}/assets/systemd.service=/usr/lib/systemd/system/smart_led.service
        )

if (FPM_EXECUTABLE)
    add_custom_command(TARGET smart_led
            POST_BUILD
            COMMAND ${FPM_EXECUTABLE} ${FPM_ARGS}
            COMMENT "Building pkg..."
            )
endif ()
