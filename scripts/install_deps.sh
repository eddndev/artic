#!/bin/bash

# Artic Dependency Installer

set -e

echo "📦 Installing Artic dependencies..."

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    OS="windows"
else
    echo -e "${RED}Unsupported OS: $OSTYPE${NC}"
    exit 1
fi

echo "Detected OS: $OS"

# Install vcpkg if not present
if [ ! -d "vcpkg" ]; then
    echo -e "${YELLOW}Installing vcpkg...${NC}"
    git clone https://github.com/microsoft/vcpkg.git

    if [ "$OS" == "windows" ]; then
        ./vcpkg/bootstrap-vcpkg.bat
    else
        ./vcpkg/bootstrap-vcpkg.sh
    fi
fi

# Install dependencies
echo -e "${YELLOW}Installing dependencies via vcpkg...${NC}"
./vcpkg/vcpkg install

# Install Node.js dependencies for runtime
echo -e "${YELLOW}Installing Node.js dependencies...${NC}"
cd runtime/client
npm install
cd ../..

echo -e "${GREEN}✅ Dependencies installed successfully!${NC}"
echo ""
echo "Next steps:"
echo "  1. Run ./scripts/build.sh to build the compiler"
echo "  2. Run ./scripts/test.sh to run tests"
