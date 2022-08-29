
include(FetchContent)

FetchContent_Declare(pi2c_source
        GIT_REPOSITORY https://github.com/JohnnySheppard/Pi2C
        GIT_TAG 1a2d0d694164affa3b5b5d616c47f08c0dc4e027
        )
FetchContent_MakeAvailable(pi2c_source)

add_library(pi2c STATIC
        ${pi2c_source_SOURCE_DIR}/pi2c.cpp
        )
target_include_directories(pi2c PUBLIC ${pi2c_source_SOURCE_DIR})
