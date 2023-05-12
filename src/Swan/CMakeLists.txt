project(libswan)

add_library(libswan STATIC
    SW_network.c
)

target_include_directories(libswan PUBLIC ./)
target_link_libraries(libswan m)