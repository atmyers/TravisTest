#!/bin/bash

# build the Doxygen documentation
doxygen Doxyfile
mkdir out/doxygen
mv html/* out/doxygen/

# now do sphinx
cd Docs
make html
mv build/html/* ../out/
cd ..
