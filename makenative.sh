#!/bin/bash

# prepare

set -e

export CLEANPATH="$PATH"
export MAKEFLAGS="-j `grep -c ^processor /proc/cpuinfo`"

# check if tools were built

if [ ! -f toolchain/bin/x86_64-echidnaos-gcc ]; then
    printf "You must build the x86_64-echidnaos tools first! (./maketools.sh)\n"
    exit 1
fi

# make sure env is clean

printf "Cleaning up\n"
rm -rf native build-* autotools automake-1.12 autoconf-2.65 automake-1.11.6 autoconf-2.64 perl520 perl-5.20.3 newlib-2.5.0 binutils-2.28 gcc-7.1.0
sync

# download prerequisites

if [ ! -f binutils-2.28.tar.bz2 ]; then
    wget "ftp://ftp.gnu.org/gnu/binutils/binutils-2.28.tar.bz2"
fi
if [ ! -f gcc-7.1.0.tar.bz2 ]; then
    wget "ftp://ftp.gnu.org/gnu/gcc/gcc-7.1.0/gcc-7.1.0.tar.bz2"
fi
if [ ! -f automake-1.12.tar.gz ]; then
    wget "http://ftp.gnu.org/gnu/automake/automake-1.12.tar.gz"
fi
if [ ! -f autoconf-2.65.tar.gz ]; then
    wget "http://ftp.gnu.org/gnu/autoconf/autoconf-2.65.tar.gz"
fi
if [ ! -f automake-1.11.6.tar.gz ]; then
    wget "http://ftp.gnu.org/gnu/automake/automake-1.11.6.tar.gz"
fi
if [ ! -f autoconf-2.64.tar.gz ]; then
    wget "http://ftp.gnu.org/gnu/autoconf/autoconf-2.64.tar.gz"
fi
if [ ! -f perl-5.20.3.tar.gz ]; then
    wget "http://www.cpan.org/src/5.0/perl-5.20.3.tar.gz"
fi
if [ ! -f newlib-2.5.0.tar.gz ]; then
    wget "ftp://sourceware.org/pub/newlib/newlib-2.5.0.tar.gz"
fi

sha256sum -c sha256packages

# compile perl for the autotools

tar -xvf perl-5.20.3.tar.gz
export PREFIX="`pwd`/perl520"
cd perl-5.20.3
./Configure -des -Dprefix="$PREFIX"
make
make install
cd ..
unset PREFIX

# build autotools for newlib

export PATH="`pwd`/perl520/bin:$CLEANPATH"
mkdir autotools
tar -xvf automake-1.12.tar.gz
tar -xvf autoconf-2.65.tar.gz
export PREFIX="`pwd`/autotools"
mkdir build-automake && cd build-automake && ../automake-1.12/configure --prefix="$PREFIX" && make && make install && cd ..
mkdir build-autoconf && cd build-autoconf && ../autoconf-2.65/configure --prefix="$PREFIX" && make && make install && cd ..
export PATH="$CLEANPATH"
unset PREFIX

# patch and build newlib

export PATH="`pwd`/autotools/bin:$CLEANPATH"
tar -xvf newlib-2.5.0.tar.gz
cp -rv newlib-patch/* newlib-2.5.0/
pushd newlib-2.5.0/newlib/libc/sys
autoconf
popd
pushd newlib-2.5.0/newlib/libc/sys/echidnaos
autoreconf
popd
mkdir native
export DESTDIR="`pwd`/native"
export PATH="`pwd`/toolchain/bin:$CLEANPATH"
mkdir build-newlib
cd build-newlib
../newlib-2.5.0/configure --prefix=/usr --target=x86_64-echidnaos
make all
make DESTDIR="$DESTDIR" install
cd ..
cp -rv $DESTDIR/usr/x86_64-echidnaos/* "$DESTDIR/usr/"
unset DESTDIR
export PATH="$CLEANPATH"

# autotools part 2

rm -rf autotools build-automake build-autoconf automake-1.11.6 autoconf-2.64
export PATH="`pwd`/perl520/bin:$CLEANPATH"
mkdir autotools
tar -xvf automake-1.11.6.tar.gz
tar -xvf autoconf-2.64.tar.gz
export PREFIX="`pwd`/autotools"
mkdir build-automake && cd build-automake && ../automake-1.11.6/configure --prefix="$PREFIX" && make && make install && cd ..
mkdir build-autoconf && cd build-autoconf && ../autoconf-2.64/configure --prefix="$PREFIX" && make && make install && cd ..
export PATH="$CLEANPATH"
unset PREFIX

# build target toolchain

export PREFIX="`pwd`/native"
export TARGET=x86_64-echidnaos
export HOST=x86_64-echidnaos
export PATH="`pwd`/toolchain/bin:$CLEANPATH"

tar -xvf binutils-2.28.tar.bz2
cp -rv binutils-patch/* binutils-2.28/
export OLDPATH="$PATH"
export PATH="`pwd`/autotools/bin:$PATH"
pushd binutils-2.28/ld
automake
popd
export PATH="$OLDPATH"
unset OLDPATH
mkdir build-binutils
cd build-binutils
../binutils-2.28/configure --target=$TARGET --host=$HOST --prefix="$PREFIX" --with-sysroot="$PREFIX" --disable-werror --disable-nls
make
make install
cd ..

tar -xvf gcc-7.1.0.tar.bz2
cd gcc-7.1.0 && contrib/download_prerequisites && cd ..
chmod 777 gcc-7.1.0/mpc-1.0.3/config.sub
cp -rv gcc-patch/* gcc-7.1.0/
sed -i 's/PICFLAG = @PICFLAG@//g' gcc-7.1.0/gcc/Makefile.in
export OLDPATH="$PATH"
export PATH="`pwd`/autotools/bin:$PATH"
pushd gcc-7.1.0/libstdc++-v3
autoconf
popd
export PATH="$OLDPATH"
unset OLDPATH
mkdir build-gcc
cd build-gcc
../gcc-7.1.0/configure --target=$TARGET --host=$HOST --prefix="$PREFIX" --with-sysroot="$PREFIX" --enable-languages=c,c++ --with-newlib --disable-nls --disable-shared --disable-host-shared --disable-default-pie
make all-gcc all-target-libgcc
make install-gcc install-target-libgcc
cd ..

unset PREFIX
unset TARGET
unset HOST
export PATH="$CLEANPATH"

# build libstdc++

export PREFIX="`pwd`/native"
export TARGET=x86_64-echidnaos
export HOST=x86_64-echidnaos
export PATH="`pwd`/toolchain/bin:$CLEANPATH"
cd build-gcc
make all-target-libstdc++-v3
make install-target-libstdc++-v3
cd ..
unset PREFIX
unset TARGET
unset HOST
export PATH="$CLEANPATH"

touch native/lib/gcc/x86_64-echidnaos/7.1.0/specs

# cleanup

printf "Cleaning up\n"
rm -rf build-* autotools automake-1.12 autoconf-2.65 automake-1.11.6 autoconf-2.64 perl520 perl-5.20.3 newlib-2.5.0 binutils-2.28 gcc-7.1.0
sync

printf "Done!\n"
