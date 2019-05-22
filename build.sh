#!/bin/bash

make

if [ $? -ne 0 ]; then
	exit $?
fi

sudo make install

if [ $? -ne 0 ]; then
        exit $?
fi

make clean

if [ $? -ne 0 ]; then
        exit 0
fi
