#!/bin/bash

# Artic Code Formatter

set -e

echo "🎨 Formatting C++ code..."

# Check if clang-format is installed
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format not found"
    echo "Install with: sudo apt install clang-format"
    exit 1
fi

# Format all C++ files
find src -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) \
    -exec clang-format -i {} \;

find tests -type f \( -name "*.cpp" -o -name "*.h" \) \
    -exec clang-format -i {} \;

echo "✅ Code formatting complete!"
