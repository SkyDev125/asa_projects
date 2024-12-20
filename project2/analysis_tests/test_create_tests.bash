# FILE: asa_projects/project1/stress_tests/test_create_tests.bash

#!/bin/bash

# Create directories
mkdir -p le_growth

# Create tests 1_1, 10_10, 20_20, 30_30 until 1000_1000
for size in {100..10000..100}; do
    # Number of stations, Number of connections, Number of lines, 1
    /workspace/university/2nd_year/1st_sem/asa/asa_projects/project2/analysis_tests/p2_gerador 200 $size $size 1 $size >le_growth/output_200_${size}_${size}.in
done
