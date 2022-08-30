
include(FetchContent)

set(Rust_CARGO_TARGET aarch64-unknown-linux-gnu)

set(ARP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/arp)
set(ARP_OUT ${CMAKE_CURRENT_BINARY_DIR}/arp)
set(ARP_INCLUDE ${ARP_OUT}/include)
set(ARP_MAIN_HEADER ${ARP_INCLUDE}/arp.h)

file(MAKE_DIRECTORY ${ARP_INCLUDE})

FetchContent_Declare(
        Corrosion
        GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
        GIT_TAG v0.2.1
)
FetchContent_MakeAvailable(Corrosion)

corrosion_import_crate(MANIFEST_PATH ${ARP_DIR}/Cargo.toml)

set(BUILD_HEADERS_ARGS
        run
        --manifest-path ${ARP_DIR}/Cargo.toml
        --features headers
        --example generate-headers
        )
add_custom_command(
        OUTPUT ${ARP_MAIN_HEADER}
        WORKING_DIRECTORY ${ARP_INCLUDE}
        COMMAND cargo ${BUILD_HEADERS_ARGS}
)

add_custom_target(arp-headers DEPENDS ${ARP_MAIN_HEADER})

add_dependencies(arp arp-headers)
set_target_properties(arp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ARP_INCLUDE})
