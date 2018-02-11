#!/bin/bash

set -e
set -x

rm -rf gcc-7.1.0 binutils-2.28 build-gcc build-binutils
rm -f gcc-7.1.0.tar.bz2 binutils-2.28.tar.bz2
rm -rf build-gcc/
rm -rf build-binutils/
wget ftp://ftp.gnu.org/gnu/gcc/gcc-7.1.0/gcc-7.1.0.tar.bz2
wget ftp://ftp.gnu.org/gnu/binutils/binutils-2.28.tar.bz2
sha256sum -c sha256packages --ignore-missing
tar -vxf gcc-7.1.0.tar.bz2
tar -vxf binutils-2.28.tar.bz2
export MAKEFLAGS="-j `grep -c ^processor /proc/cpuinfo`" && export PREFIX="`pwd`/tools" && export TARGET=x86_64-elf && export PATH="$PREFIX/bin:$PATH" && mkdir build-binutils && cd build-binutils && ../binutils-2.28/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror && make && make install && cd ../gcc-7.1.0 && contrib/download_prerequisites && cd .. && mkdir build-gcc && cd build-gcc && ../gcc-7.1.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers && make all-gcc && make all-target-libgcc && make install-gcc && make install-target-libgcc
