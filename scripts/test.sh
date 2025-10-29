#!/bin/bash

# Artic Test Runner

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "🧪 Running Artic Tests..."

# Check if build exists
if [ ! -d "build" ]; then
    echo -e "${RED}Error: build directory not found${NC}"
    echo "Run ./scripts/build.sh first"
    exit 1
fi

cd build

# Run tests
echo -e "${YELLOW}Running test suite...${NC}"
ctest --output-on-failure --verbose

# Check result
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
fi
