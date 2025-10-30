#!/bin/bash

echo "Installing Artic Language Extension for VS Code..."
echo ""

EXTENSION_DIR="$HOME/.vscode/extensions/artic-language"

if [ -d "$EXTENSION_DIR" ]; then
    echo "Removing existing extension..."
    rm -rf "$EXTENSION_DIR"
fi

echo "Creating extension directory..."
mkdir -p "$EXTENSION_DIR"

echo "Copying files..."
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cp -r "$SCRIPT_DIR"/* "$EXTENSION_DIR/"

echo ""
echo "Installation complete!"
echo ""
echo "Please reload VS Code to activate the extension:"
echo "1. Press Ctrl+Shift+P (or Cmd+Shift+P on macOS)"
echo "2. Type 'Reload Window' and press Enter"
echo ""
echo "Or simply restart VS Code."
echo ""
