#!/bin/bash

./file2c -c -n logo -f logo.bmp logo.bmp romfs_logo.h
./file2c -c -n digit_0 -f 0.bmp 0.bmp romfs_0.h
./file2c -c -n digit_1 -f 1.bmp 1.bmp romfs_1.h
./file2c -c -n digit_2 -f 2.bmp 2.bmp romfs_2.h
./file2c -c -n digit_3 -f 3.bmp 3.bmp romfs_3.h
./file2c -c -n digit_4 -f 4.bmp 4.bmp romfs_4.h
./file2c -c -n digit_5 -f 5.bmp 5.bmp romfs_5.h
./file2c -c -n digit_6 -f 6.bmp 6.bmp romfs_6.h
./file2c -c -n digit_7 -f 7.bmp 7.bmp romfs_7.h
./file2c -c -n digit_8 -f 8.bmp 8.bmp romfs_8.h
./file2c -c -n digit_9 -f 9.bmp 9.bmp romfs_9.h

# LinuxFr logo
./file2c -c -n linuxfr -f linuxfr.bmp linuxfr.bmp romfs_linuxfr.h

