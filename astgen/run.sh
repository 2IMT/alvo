#!/bin/sh

cp ../src/ast.h ../src/ast.h.bak
cp ../src/ast.cpp ../src/ast.cpp.bak
python3 astgen.py \
    --schema_file ../src/ast.json \
    --clang-format-config ../.clang-format \
    --output-dir ../src
