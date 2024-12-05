# FILE: asa_projects/project1/stress_tests/test_create_tests.bash

#!/bin/bash

# Create directories
mkdir -p same_growth

# Create tests 1_1, 10_10, 20_20, 30_30 until 1000_1000
for size in {10..1000..10}; do
    /workspace/university/2nd_year/1st_sem/asa/asa_projects/project1/stress_tests/a.out $size $size >same_growth/output_${size}_${size}.in
done
