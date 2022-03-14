#!/bin/bash

set -x
clang -Wall -Wextra -Wpedantic -std=c99 -O test36.c
./a.out | node -
rm -i ./a.out
