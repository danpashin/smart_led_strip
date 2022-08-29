find_program(MAKE_EXECUTABLE NAMES gmake make REQUIRED)

include(ExternalProject)

set(LIBOPING_VERSION 1.10.0)
set(LIBOPING_DIR ${CMAKE_CURRENT_BINARY_DIR}/opt/liboping)
set(LIBOPING_BIN ${CMAKE_CURRENT_BINARY_DIR}/liboping)
set(LIBOPING_STATIC_LIB ${LIBOPING_BIN}/lib/liboping.a)
set(LIBOPING_INCLUDES ${LIBOPING_BIN}/include)

file(MAKE_DIRECTORY ${LIBOPING_INCLUDES})

ExternalProject_Add(liboping
        URL "https://github.com/octo/liboping/archive/refs/tags/liboping-${LIBOPING_VERSION}.tar.gz"
        PREFIX ${LIBOPING_BIN}
        SOURCE_DIR ${LIBOPING_DIR}
        CONFIGURE_COMMAND ${LIBOPING_DIR}/configure
        --enable-static=yes --disable-shared --without-perl-bindings --prefix=${LIBOPING_BIN}
        --host aarch64-linux-gnu

        BUILD_COMMAND ${MAKE_EXECUTABLE} CFLAGS=-Wno-error
        INSTALL_COMMAND ${MAKE_EXECUTABLE} install
        BUILD_BYPRODUCTS ${LIBOPING_STATIC_LIB}
        )
ExternalProject_Add_Step(liboping
        bootstrap
        WORKING_DIRECTORY ${LIBOPING_DIR}
        COMMAND ./autogen.sh
        DEPENDEES download
        DEPENDERS configure
        )

add_library(oping STATIC IMPORTED GLOBAL)
add_dependencies(oping liboping)

set_target_properties(oping PROPERTIES IMPORTED_LOCATION ${LIBOPING_STATIC_LIB})
set_target_properties(oping PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LIBOPING_INCLUDES})
