cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(Swan VERSION 1.0.0 DESCRIPTION "A basic framework for creation of neural networks" LANGUAGES C)

set(CMAKE_BUILD_TYPE "Debug")

add_subdirectory(src)