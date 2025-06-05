#!/bin/bash
# Run clang-tidy locally

# Check if clang-tidy is installed
if ! command -v clang-tidy &> /dev/null; then
    echo "clang-tidy is not installed. Please install it first:"
    echo "  Ubuntu/Debian: sudo apt-get install clang-tidy"
    echo "  macOS: brew install llvm"
    echo "  Arch Linux: sudo pacman -S clang"
    exit 1
fi

# Generate compile_commands.json
echo "Generating compile_commands.json..."
pio run -t compiledb -e native

# Check if compile_commands.json exists
if [ ! -f "compile_commands.json" ]; then
    echo "Error: compile_commands.json not found!"
    exit 1
fi

# Run clang-tidy
echo "Running clang-tidy..."
find src include -name "*.cpp" -o -name "*.h" | \
xargs clang-tidy -p . --fix

echo "Done! Check git diff for automatic fixes."