#!/bin/bash

echo "Compiling deiGo program $1 to a.out"

./gocompiler < $1 > out.ll
llc out.ll
clang-7 out.ll