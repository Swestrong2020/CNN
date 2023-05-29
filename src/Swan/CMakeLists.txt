project(swan)

add_subdirectory(matrix)

add_library(swan STATIC
    SW_network.c
)

target_include_directories(swan PUBLIC ./)
target_link_libraries(swan swanmatrix)
target_link_libraries(swan m)