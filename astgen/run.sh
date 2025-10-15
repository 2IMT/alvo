#!/bin/sh

python3 astgen.py \
    --schema_file ../src/ast.json \
    --clang-format-config ../.clang-format \
    --output-dir ../src
