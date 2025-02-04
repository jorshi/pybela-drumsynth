#!/bin/bash

# check if pytorch is in /opt/ in the docker

if [ ! -d "/sysroot/opt/pytorch-install" ]; then
    echo "Pytorch not found in /sysroot/opt/pytorch-install"

    # Download pytorch to /sysroot/opt/torch
    url=https://github.com/pelinski/bela-torch/releases/download/v2.5.1/pytorch-v2.5.1.tar.gz
    echo "Downloading Pytorch from $url"
    wget -O - $url | tar -xz -C /sysroot/opt/pytorch-install
fi

echo "Building pybela DrumSynth..."

cmake -S . -B build \
-DPROJECT_NAME=pybela-drumsynth-inference \
-DCMAKE_TOOLCHAIN_FILE=../Toolchain.cmake \
-DCMAKE_SYSROOT=/sysroot/root/Bela/
# -DCMAKE_CURRENT_SOURCE_DIR=$(pwd)/src

cmake --build build -j

echo "Copying libbelafull to Bela..."
rsync \
--timeout=10 \
-avzP  /sysroot/root/Bela/lib/libbelafull.so \
root@$BBB_HOSTNAME:Bela/lib/libbelafull.so

echo "Copying libtorch to Bela..."
rsync \
--timeout=10 \
-avzP  /sysroot/opt/pytorch-install/lib/libtorch_cpu.so /sysroot/opt/pytorch-install/lib/libtorch.so /sysroot/opt/pytorch-install/lib/libc10.so root@$BBB_HOSTNAME:Bela/lib/

echo "Copying drumsynth project to Bela..."
rsync \
--timeout=10 \
-avzP build/pybela-drumsynth-inference \
root@$BBB_HOSTNAME:~/Bela/projects/pybela-drumsynth-inference/