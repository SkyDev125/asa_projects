# FILE: asa_projects/project1/stress_tests/test_create_tests.bash

#!/bin/bash

# Create tests 1_1, 10_10, 20_20, 30_30 until 1000_1000
for size in {1..1000}; do
    ./p2_gerador 20 50 4 1 ${size} >test${size}.in
done
