cmake_minimum_required(VERSION 3.24)
project(smart_led)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
set(CMAKE_SYSROOT /usr/aarch64-linux-gnu)

include(FetchContent)
FetchContent_Declare(
        Pi2C_Source
        GIT_REPOSITORY https://github.com/JohnnySheppard/Pi2C
        GIT_TAG 1a2d0d694164affa3b5b5d616c47f08c0dc4e027
)

FetchContent_MakeAvailable(Pi2C_Source)

add_library(Pi2C STATIC ${pi2c_source_SOURCE_DIR}/pi2c.cpp)

add_executable(smart_led
        src/main.cpp
        src/arduino.cpp
        )
target_include_directories(smart_led
        PRIVATE
        include
        ${pi2c_source_SOURCE_DIR}
        )
target_link_libraries(smart_led Pi2C)
