#!/bin/bash

yum groupinstall "Development Tools" -y
sudo yum -y install libyaml-devel libffi-devel openssl-devel
sudo yum install libcurl-devel -y

mkdir -p cmake-3.25 && wget -qO- "https://cmake.org/files/v3.25/cmake-3.25.0-linux-x86_64.tar.gz" | tar --strip-components=1 -xz -C cmake-3.25
export PATH=`pwd`/cmake-3.25/bin:$PATH

git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
mkdir sdk_build && cd sdk_build
cmake ../aws-sdk-cpp -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/usr/local/ -DCMAKE_INSTALL_PREFIX=/usr/local/ -DBUILD_ONLY="s3"
make
make install

cd ..

git clone https://github.com/MaxiMittel/lambdadb
mkdir build && cd build
cmake ../lambdadb -DCMAKE_BUILD_TYPE=Release -DRUNTIME=AWS_EC2
make