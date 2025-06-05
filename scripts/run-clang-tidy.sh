#!/bin/bash
# Run clang-tidy locally

# Generate compile_commands.json
echo "Generating compile_commands.json..."
pio run -t compiledb -e native
cp .pio/build/native/compile_commands.json .

# Run clang-tidy
echo "Running clang-tidy..."
find src include -name "*.cpp" -o -name "*.h" | \
xargs clang-tidy -p . --fix

echo "Done! Check git diff for automatic fixes."