# FILE: asa_projects/project1/stress_tests/test_create_tests.bash

#!/bin/bash

# Loop through table sizes from 1 to 1000
# Loop through sequence sizes from 1 to 1000
mkdir cube_squared_growth
for sequence_size in {1..10}; do
    table_size=$((sequence_size ** 2))
    seq_size=$((sequence_size ** 3))
    # Call the generator script with the current table size and sequence size
    /workspace/university/2nd_year/1st_sem/asa/asa_projects/project1/stress_tests/generator $table_size $seq_size >cube_squared_growth/output_${table_size}_${seq_size}.in
done
