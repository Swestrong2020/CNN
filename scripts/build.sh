#!/usr/bin/env bash

# location of the scripts/ directory
BASEDIR=$(dirname "$0")

cd $(BASEDIR)/../build

make

cd ..

./build/src/main