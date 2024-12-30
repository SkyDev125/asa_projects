#!/bin/bash

# Define the number of tests to generate
NUM_TESTS=100

# Create directories for the generated tests if they don't exist
mkdir -p generated_tests/factory_growth
mkdir -p generated_tests/country_growth
mkdir -p generated_tests/children_growth
mkdir -p generated_tests/all_growth

# Generate tests for factory growth
for i in $(seq 15 5 510); do
    python3 p3_gerador.py $i 10 10 0 10 $i > generated_tests/factory_growth/test_${i}_10_10.txt
done

# Generate tests for country growth
for i in $(seq 15 5 510); do
    python3 p3_gerador.py 10 $i 10 0 10 10 > generated_tests/country_growth/test_10_${i}_10.txt
done

# Generate tests for children growth
for i in $(seq 15 5 510); do
    python3 p3_gerador.py 10 10 $i 0 $i 10 > generated_tests/children_growth/test_10_10_${i}.txt
done

# Generate tests for all growth
for i in $(seq 10 5 505); do
    python3 p3_gerador.py $i $i $i 0 $i $i > generated_tests/all_growth/test_${i}_${i}_${i}.txt
done

echo "All tests generated successfully."
