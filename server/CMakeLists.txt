cmake_minimum_required(VERSION 3.24)
project(smart_led
        VERSION 0.0.1
        DESCRIPTION "Raspberry PI server to control arduino led strip"
        HOMEPAGE_URL "https://github.com/danpashin/smart_led_strip"
        LANGUAGES CXX C
        )
include(FetchContent)

find_program(FPM "fpm")
if (NOT FPM)
    message(FATAL_ERROR "fpm is required to build. Please check you have it installed with 'gem install fpm'")
endif ()

set(PROJECT_AUTHOR danpashin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -Wpedantic)

# Hardcoded as project is only available for rpi
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_SYSROOT /usr/aarch64-linux-gnu)

FetchContent_Declare(Pi2C_Source
        GIT_REPOSITORY https://github.com/JohnnySheppard/Pi2C
        GIT_TAG 1a2d0d694164affa3b5b5d616c47f08c0dc4e027
        )
FetchContent_MakeAvailable(Pi2C_Source)
add_library(Pi2C STATIC ${pi2c_source_SOURCE_DIR}/pi2c.cpp)

FetchContent_Declare(liboping_source
        GIT_REPOSITORY https://github.com/octo/liboping
        GIT_TAG 656ea6996d083090cbbae1581287a3b1a5dcfb56
        )
FetchContent_MakeAvailable(liboping_source)
add_library(oping STATIC ${liboping_source_SOURCE_DIR}/src/liboping.c)
target_compile_definitions(oping PRIVATE
        STDC_HEADERS
        HAVE_STDINT_H
        HAVE_UNISTD_H
        HAVE_FCNTL_H
        HAVE_SYS_TYPES_H
        HAVE_SYS_STAT_H
        TIME_WITH_SYS_TIME
        HAVE_SYS_SOCKET_H
        HAVE_NETDB_H
        HAVE_NETINET_IN_SYSTM_H
        HAVE_NETINET_IN_H
        HAVE_NETINET_IP_H
        HAVE_NETINET_IP_ICMP_H
        HAVE_NETINET_IP6_H
        HAVE_NETINET_ICMP6_H
        )

add_executable(smart_led
        src/main.cpp
        src/arduino.cpp
        src/color.cpp
        src/day_part.cpp
        )
target_include_directories(smart_led PRIVATE
        include
        ${pi2c_source_SOURCE_DIR}
        ${liboping_source_SOURCE_DIR}/src
        )
target_link_libraries(smart_led Pi2C oping)

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

add_custom_command(TARGET smart_led
        POST_BUILD
        COMMAND ${FPM} ${FPM_ARGS}
        COMMENT "Building pkg..."
        )

