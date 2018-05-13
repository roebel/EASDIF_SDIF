#! /bin/bash

./autogen.sh

if [ `uname` = "Darwin" -o `uname` =  "Linux" ];then
flags=$flags
else
export CXXFLAGS="$CFLAGS -mno-cygwin" 
export CFLAGS="$CFLAGS -mno-cygwin"
export LDFLAGS="$LDFLAGS -mno-cygwin"
fi
set -x
if [ "$1" = "" ]; then
./configure $configflags --prefix=/u/formes/share --libdir=/u/formes/share/lib/$SYS --bindir=/u/formes/share/bin/$SYS
else
./configure $configflags --prefix="$1" --libdir="$1"/lib/$SYS --bindir="$1"/bin/$SYS
fi
make
make install
