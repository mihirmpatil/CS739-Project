#!/bin/bash
echo "parameters: number_of_iterations max_size_of_buffer number_of_iterations_for_mem_test"
for (( c=20480; c <= 100000; c+=1000))
do
    ./cache_test 1 $c 1 $3
done
