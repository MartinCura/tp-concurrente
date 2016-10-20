# /usr/bin/env sh
mkdir build
cp ./conf.json ./build/conf.json
cd build
cmake ..
make
