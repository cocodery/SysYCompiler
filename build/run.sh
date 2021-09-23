#!/bin/bash

echo "make -j4"

make -j4

echo "./Compiler -S -o test.s test.sy -O2"

echo "compiler log"

./compiler -S -o test.s test.sy -O2

echo "Hello World!"

