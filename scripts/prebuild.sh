#!/usr/bin/env bash

# location of the scripts/ directory
BASEDIR=$(dirname "$0")

cd $(BASEDIR)/../build

cmake -G "MinGW Makefiles" ../CMakeLists.txt