#!/bin/sh

SOURCES=$(find ./src -type f -name '*.cpp')

clang-tidy $SOURCES -extra-arg=-std=c++20
