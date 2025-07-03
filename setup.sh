#!/bin/bash
set -e

echo "==> [1/9] Checking architecture..."
ARCH=$(uname -m)
echo "    Detected ARCH: $ARCH"

PROJECT_DIR=ultimate-alpr-recognizer
SDK_DIR=ultimateALPR-SDK
SDK_BIN_DIR=$SDK_DIR/binaries/linux/$ARCH

echo "==> [2/9] Cloning your recognizer project..."
if [ ! -d "$PROJECT_DIR" ]; then
    git clone https://github.com/innoguy/ultimate-alpr-recognizer.git
else
    echo "    Project directory exists. Skipping clone."
fi
cd $PROJECT_DIR

echo "==> [3/9] Installing required packages..."
apt update
apt install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    libopencv-dev \
    pkg-config \
    nlohmann-json3-dev \
    libgtk-3-dev \
    libgl1-mesa-glx

if [[ "$ARCH" == "x86_64" ]]; then
    echo "==> [4/9] Installing Intel OpenVINO basekit..."
    wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB \
        | gpg --dearmor | tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null

    echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" \
        | tee /etc/apt/sources.list.d/oneAPI.list

    apt update
    apt install -y intel-basekit
else
    echo "==> [!] Skipping OpenVINO basekit (unsupported on $ARCH)"
fi

echo "==> [5/9] Cloning ultimateALPR SDK (if not exists)..."
if [ ! -d "$SDK_DIR" ]; then
    git clone https://github.com/DoubangoTelecom/ultimateALPR-SDK.git
else
    echo "    SDK directory exists. Skipping clone."
fi

cd $SDK_BIN_DIR
if [ ! -f "libtensorflow.so" ]; then
    echo "==> [6/9] Downloading TensorFlow runtime..."
    wget https://doubango.org/deep_learning/libtensorflow_r1.14_cpu+gpu_linux_x86-64.tar.gz
    tar -xzf libtensorflow_r1.14_cpu+gpu_linux_x86-64.tar.gz
    rm libtensorflow_r1.14_cpu+gpu_linux_x86-64.tar.gz
else
    echo "    TensorFlow binaries already exist."
fi
cd ../../../..

echo "==> [7/9] Building project..."
mkdir -p build && cd build
cmake ..
make -j$(nproc)
cd ..

echo "==> [8/9] Verifying build..."
if [ -f "build/recognizer" ]; then
    echo "    ✅ Build successful!"
else
    echo "    ❌ Build failed. Check errors above."
    exit 1
fi

echo "==> [9/9] Example run command:"
echo "---------------------------------------------"
echo "LD_LIBRARY_PATH=./ultimateALPR-SDK/binaries/linux/"$ARCH "./build/recognizer --image ./testData/multi.jpg --assets ./ultimateALPR-SDK/assets/ "
echo "---------------------------------------------"
