#!/bin/bash

# Artic Build Script

set -e

echo "🔨 Building Artic Compiler..."

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Parse arguments
BUILD_TYPE=${1:-Release}
BUILD_TESTS=${2:-ON}

echo "Build type: ${BUILD_TYPE}"
echo "Build tests: ${BUILD_TESTS}"

# Create build directory
mkdir -p build

# Configure
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DARTIC_BUILD_TESTS=${BUILD_TESTS} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build
echo -e "${YELLOW}Building...${NC}"
cmake --build build --config ${BUILD_TYPE} -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Success
echo -e "${GREEN}✅ Build completed successfully!${NC}"
echo ""
echo "Binary location: build/artic"
echo "Run tests: cd build && ctest"
echo "Install: sudo cmake --install build"
