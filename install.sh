#!/usr/bin/env bash

set -e

# Step 1: Create ~/.config/huegen if it doesn't exist
echo "[+] Ensuring config directory exists..."
mkdir -p ~/.config/huegen

# Step 2: Copy templates and themes
echo "[+] Copying config templates to ~/.config/huegen/"
cp -r ./config/* ~/.config/huegen/

# Step 3: Clean and build the project
echo "[+] Building the project..."
rm -rf build
mkdir build
cd build
cmake ..
make -j$(nproc)
cd ..

# Step 4: Copy binary to ~/.local/bin
echo "[+] Installing binary to ~/.local/bin"
mkdir -p ~/.local/bin
cp build/heugen ~/.local/bin/

# Step 5: Ensure ~/.local/bin is in PATH
if [[ ":$PATH:" != *":$HOME/.local/bin:"* ]]; then
  echo "[!] ~/.local/bin is not in your PATH. Add this to your shell profile:"
  echo 'export PATH="$HOME/.local/bin:$PATH"'
else
  echo "[✓] Installed successfully. Run with: heugen"
fi
