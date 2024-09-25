#!/bin/bash

# stop the execution if any error
set -e

export PWD_PATH="$(pwd)"
export KERNEL_PATH="$PWD_PATH/kernel_linux_101/linux-6.10.11"
export BUSYBOX_PATH="$PWD_PATH/busybox-1.36.1/"
export ENV_PATH="$PWD_PATH/env/"

# Add LKM to be compiled
cp -r env/kplayground/ $KERNEL_PATH/drivers/

# Check if Makefile will compile the module
makefile_path="$KERNEL_PATH/drivers/Makefile"
if [ -f "$makefile_path" ] && !(grep -q "kplayground" "$makefile_path"); then
    echo "obj-m += kplayground/" >> "$makefile_path"
fi

cd $KERNEL_PATH
make -j$(nproc)

cp $KERNEL_PATH/drivers/kplayground/kplayground.ko $ENV_PATH/initramfs/
cp $KERNEL_PATH/arch/x86/boot/bzImage $ENV_PATH
cp $KERNEL_PATH/vmlinux $ENV_PATH

cd $ENV_PATH
./compress.sh
./run.sh
