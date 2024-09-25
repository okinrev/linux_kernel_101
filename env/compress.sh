#!/bin/sh

cd script_userland

gcc -o userland -static userland.c -D_GNU_SOURCE
cp ./userland ../initramfs/userland

cd ../initramfs
find . -print0 \
| cpio --null -ov --format=newc \
| gzip -9 > initramfs.cpio.gz
mv ./initramfs.cpio.gz ../
cd ..
