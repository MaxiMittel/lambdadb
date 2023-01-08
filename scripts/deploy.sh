#!/bin/bash

# Build lambda runtime
if [ -d "../build-lambda" ]; then
    rm -rf ../build-lambda
fi

mkdir ../build-lambda
cd ../build-lambda
cmake .. -DCMAKE_BUILD_TYPE=Release -DRUNTIME=AWS_LAMBDA
make
make aws-lambda-package-lambdadb

# Array of memory sizes
declare -a memorySizes=(
    "128"
    "256"
    "512"
    "1024"
    "2048"
    "3008"
)

# Update all lambdas
for memorySize in "${memorySizes[@]}"
do
    echo "Updating lambda with memory size $memorySize"
    aws lambda update-function-code --function-name lambdadb-func-$memorySize --zip-file fileb://lambdadb.zip --region eu-central-1
done