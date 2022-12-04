# LambdaDB

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Run

```bash
./lambdadb
```

## Install AWS SDK
    
```bash
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
mkdir sdk_build
cd sdk_build
cmake ../aws-sdk-cpp -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/usr/local/ -DCMAKE_INSTALL_PREFIX=/usr/local/ -DBUILD_ONLY="s3"
make
make install
```
