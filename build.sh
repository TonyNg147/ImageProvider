#!/bin/bash
mkdir -p build
pushd build
/home/tony/Qt/6.2.7/gcc_64/bin/qmake6 ../
make -j8
popd
# exec ./build/ImageProvider -f 2 --to 9 "${PWD}/ImagesDemo" "${PWD}/ImagePool.dat"
exec ./build/ImageProvider "${PWD}/ImagePool.dat"