#!/bin/bash

# check if pytorch is in /opt/pytorch-install

if [ ! -d "/opt/pytorch-install" ]; then
    echo "Pytorch not found in /opt/pytorch-install"

    # Download pytorch to /opt/pytorch-install
    url=https://github.com/rodrigodzf/bela-torch/releases/download/master/pytorch-install.tar.gz
    echo "Downloading Pytorch from $url"
    wget -O - $url | tar -xz -C /opt
fi

echo "Building PyBela DrumSynth"

cmake -S . -B build \
-DENABLE_PYTORCH_FRONTEND=ON \
-DCMAKE_TOOLCHAIN_FILE=Toolchain.cmake

cmake --build build -j

echo "Copying files to Bela"

rsync \
--timeout=10 \
-avzP  /sysroot/root/Bela/lib/libbelafull.so \
root@$BBB_HOSTNAME:Bela/lib/libbelafull.so

rsync \
--timeout=10 \
-avzP  /opt/pytorch-install/lib/libc10.so \
root@$BBB_HOSTNAME:Bela/lib/libc10.so

rsync \
--timeout=10 \
-avzP  /opt/pytorch-install/lib/libtorch_cpu.so \
root@$BBB_HOSTNAME:Bela/lib/libtorch_cpu.so

rsync \
--timeout=10 \
-avzP  /opt/pytorch-install/lib/libtorch.so \
root@$BBB_HOSTNAME:Bela/lib/libtorch.so


rsync \
--timeout=10 \
-avzP build/src/pybela-drumsynth \
root@$BBB_HOSTNAME:~/Bela/projects/pybela-drumsynth-inference/

# Copy the ckpt file to Bela
rsync \
--timeout=10 \
-avzP model.jit \
root@$BBB_HOSTNAME:~/Bela/projects/pybela-drumsynth-inference/
