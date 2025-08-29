#!/bin/bash

# Script to regenerate node-clang-raw bindings using codegen
# This script:
# 1. Builds node-clang-raw
# 2. Regenerates node-clang-raw using codegen
# 3. Builds node-clang-raw again with the new bindings
# 4. Runs the codegen tests

set -e  # Exit on error

echo "Using Node version:"
node -v

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_step() {
    echo -e "${GREEN}==>${NC} $1"
}

print_error() {
    echo -e "${RED}Error:${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}Warning:${NC} $1"
}

# Get the script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

print_step "Starting regeneration process..."

# Step 1: Build node-clang-raw (initial build)
print_step "Step 1: Building node-clang-raw (initial build)..."
cd packages/node-clang-raw
npm install
if [ $? -eq 0 ]; then
    echo "  ✓ node-clang-raw built successfully"
else
    print_error "Failed to build node-clang-raw"
    exit 1
fi
cd "$SCRIPT_DIR"

# Step 2: Regenerate node-clang-raw using codegen
print_step "Step 2: Regenerating node-clang-raw bindings using codegen..."

# Create temporary directory for generated bindings
TEMP_DIR=$(mktemp -d)
print_step "Using temporary directory: $TEMP_DIR"

## Run codegen to generate new bindings using Node 24 TS stripping
## Note: Index.h includes the other headers, so we only need to parse that one
node --experimental-strip-types packages/codegen/bin/cli.ts \
    -n node-clang-raw \
    -v 0.0.1 \
    -l clang \
    -I /opt/homebrew/Cellar/llvm/20.1.8/include \
    -L /opt/homebrew/Cellar/llvm/20.1.8/lib \
    -H clang-c/Index.h \
    -o "$TEMP_DIR/node-clang-raw" \
    /opt/homebrew/Cellar/llvm/20.1.8/include/clang-c/Index.h

if [ $? -eq 0 ]; then
    echo "  ✓ Bindings generated successfully"
else
    print_error "Failed to generate bindings (Node TS)." 
    print_warning "You can try installing tsx locally and switching to it."
    rm -rf "$TEMP_DIR"
    exit 1
fi

# Back up current bindings
print_step "Backing up current bindings..."
cp packages/node-clang-raw/index.ts packages/node-clang-raw/index.ts.bak
cp packages/node-clang-raw/src/binding.cpp packages/node-clang-raw/src/binding.cpp.bak

# Copy generated files to node-clang-raw
print_step "Copying regenerated bindings..."
cp "$TEMP_DIR/node-clang-raw/index.ts" packages/node-clang-raw/index.ts
cp "$TEMP_DIR/node-clang-raw/src/binding.cpp" packages/node-clang-raw/src/binding.cpp

# Clean up temp directory
rm -rf "$TEMP_DIR"

echo "  ✓ Bindings copied successfully"

# Step 3: Build node-clang-raw again with new bindings
print_step "Step 3: Building node-clang-raw with regenerated bindings..."
cd packages/node-clang-raw

# Clean previous build
print_step "Cleaning previous build..."
npm run clean

# Rebuild with new bindings
print_step "Rebuilding..."
npm run rebuild

if [ $? -eq 0 ]; then
    echo "  ✓ node-clang-raw rebuilt successfully with new bindings"
else
    print_error "Failed to rebuild node-clang-raw"
    print_warning "Restoring backup files..."
    cp index.ts.bak index.ts
    cp src/binding.cpp.bak src/binding.cpp
    rm index.ts.bak src/binding.cpp.bak
    exit 1
fi

# Remove backup files
rm index.ts.bak src/binding.cpp.bak

cd "$SCRIPT_DIR"

# Step 4: Run codegen tests
print_step "Step 4: Running codegen tests..."
cd packages/codegen
npm test

if [ $? -eq 0 ]; then
    echo "  ✓ All tests passed"
else
    print_error "Tests failed"
    exit 1
fi

cd "$SCRIPT_DIR"

print_step "✅ Regeneration complete!"
echo ""
echo "Summary:"
echo "  1. node-clang-raw built successfully (initial)"
echo "  2. Bindings regenerated using codegen"
echo "  3. node-clang-raw rebuilt with new bindings"
echo "  4. codegen tests passed"
