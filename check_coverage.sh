#!/bin/bash
# Script to check code coverage locally

echo "1. Running tests with coverage..."
pio test -e native

echo -e "\n2. Capturing coverage data..."
lcov --capture --directory .pio/build/native/ --output-file coverage.info

echo -e "\n3. Removing unwanted files from coverage..."
lcov --remove coverage.info '/usr/*' '*/test/*' '*/lib/mocks/*' '*/.pio/*' '*/lib/test/*' --output-file coverage.info

echo -e "\n4. Generating HTML report..."
genhtml coverage.info --output-directory coverage-report

echo -e "\n5. Coverage summary:"
lcov --list coverage.info

echo -e "\nHTML report generated in: coverage-report/index.html"
echo "To view: xdg-open coverage-report/index.html"
