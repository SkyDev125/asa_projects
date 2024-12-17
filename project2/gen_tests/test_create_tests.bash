# FILE: asa_projects/project1/stress_tests/test_create_tests.bash

#!/bin/bash

# Create tests 1_1, 10_10, 20_20, 30_30 until 1000_1000
for size in {1..100}; do
    ./p2_gerador 10 10 4 >test${size}.in
done
