cmake_minimum_required(VERSION 3.24)
project(smart_led
        VERSION 0.0.1
        DESCRIPTION "Raspberry PI server to control arduino led strip"
        HOMEPAGE_URL "https://github.com/danpashin/smart_led_strip"
        LANGUAGES CXX
        )

set(PROJECT_AUTHOR danpashin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -Wpedantic)

# Hardcoded as project is only available for rpi
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_SYSROOT /usr/aarch64-linux-gnu)

find_program(FPM_EXECUTABLE NAMES fpm)
if (NOT FPM_EXECUTABLE)
    message(FATAL_ERROR "fpm is required to build. Please check you have it installed with 'gem install fpm'")
endif ()

find_program(STRIP_EXECUTABLE NAMES sstrip strip)
if (NOT STRIP_EXECUTABLE)
    message(WARNING "Either strip or sstrip should optionally be installed")
endif ()

include(cmake/libp2c.cmake)
include(cmake/arp.cmake)
include(CheckIPOSupported)

check_ipo_supported(RESULT LTO_SUPPORTED OUTPUT LTO_SUPPORT_ERROR)

add_executable(smart_led
        src/main.cpp
        src/arduino.cpp
        src/color.cpp
        src/day_part.cpp
        )
target_include_directories(smart_led PRIVATE include)
target_link_libraries(smart_led pi2c arp)

if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    if (LTO_SUPPORTED)
        message(STATUS "IPO/LTO enabled")
        set_property(TARGET smart_led PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else ()
        message(STATUS "IPO/LTO not supported: <${LTO_SUPPORT_ERROR}>")
    endif ()
endif ()

set(FPM_ARGS
        --force
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
    if (STRIP_EXECUTABLE)
        set(STRIP_SMARTLED_COMMAND ${STRIP_EXECUTABLE} $<TARGET_FILE:smart_led>)
    endif ()

    add_custom_command(TARGET smart_led
            POST_BUILD
            COMMAND ${STRIP_SMARTLED_COMMAND}
            COMMAND ${FPM_EXECUTABLE} ${FPM_ARGS}
            COMMENT "Building pkg..."
            )
endif ()
