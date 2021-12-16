#!/bin/bash

echo "Deleting old zip file..."
rm -f gocompiler.zip
cd src
echo "Creating new zip file..."
zip ../gocompiler *
cd ..
