#!/bin/bash

echo "Deleting old lex and yacc files"
make clean
echo "Deleting old zip file..."
rm -f gocompiler.zip
cd src
echo "Creating new zip file..."
zip ../gocompiler *
cd ..
