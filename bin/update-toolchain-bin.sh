#!/bin/sh
ROOT_FOLDER=`dirname $0`

cd $ROOT_FOLDER/src && tar -jcvf ../msx-toolchain-bin.tar.bz2 * && cd ..
