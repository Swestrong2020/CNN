project(swanmatrix)

add_library(swanmatrix
    SW_matrix.c
)

target_include_directories(swanmatrix PUBLIC ./)
target_link_libraries(swanmatrix m)