#!/bin/bash

# Check if Run_time_test_cases folder exists, if yes, remove it
if [ -d "Run_time_test_cases" ]; then
    rm -rf Run_time_test_cases
fi

# Create Run_time_test_cases folder
mkdir Run_time_test_cases

# Compilation
gcc -fopenmp rand_test_generator.c -o rand_test_generator.o -lm
g++ -fopenmp main.cpp netlist.cpp batch.cpp Grid_Graph.cpp -o Working_code.o

# First set of iterations
for i in $(seq 2 ); do
    case $i in
        1)
            MAX_X=50
            MAX_Y=50
            MIN_DISTANCE=5
            NUM_ROUTES=2500
            CAPACITY=12
            VIA=2
            ;;
        2)
            MAX_X=20
            MAX_Y=20
            MIN_DISTANCE=4
            NUM_ROUTES=300
            CAPACITY=6
            VIA=3
            ;;
        3)
            MAX_X=1000
            MAX_Y=1000
            MIN_DISTANCE=20
            NUM_ROUTES=800000
            CAPACITY=20
            VIA=4
            ;;
    esac

    # Determine bound box dimension for this iteration
    if (( MAX_X < 10 )); then
        BOUND_BOX_DIMENSION=7
    elif (( MAX_X < 100 )); then
        BOUND_BOX_DIMENSION=9
    elif (( MAX_X < 1000 )); then
        BOUND_BOX_DIMENSION=15
    fi

    # Execution
    ./rand_test_generator.o 8 $MAX_X $MAX_Y $MIN_DISTANCE $NUM_ROUTES $CAPACITY $VIA
    ./Working_code.o sample_test.txt ./Run_time_test_cases/sampleout_for_${i}_threads_for_batch1.txt $BOUND_BOX_DIMENSION 1 1
    echo "the iteration number is" $i
done

# Second set of iterations
for j in $(seq 1 10); do
    # Parameters for the second set of iterations
    MAX_X=1000
    MAX_Y=1000
    MIN_DISTANCE=20
    NUM_ROUTES=800000
    CAPACITY=20
    VIA=4

    # Determine bound box dimension for this iteration
    if (( MAX_X < 10 )); then
        BOUND_BOX_DIMENSION=7
    elif (( MAX_X < 100 )); then
        BOUND_BOX_DIMENSION=9
    elif (( MAX_X < 1000 )); then
        BOUND_BOX_DIMENSION=15
    fi

    # Execution
    ./rand_test_generator 8 $MAX_X $MAX_Y $MIN_DISTANCE $NUM_ROUTES $CAPACITY $VIA
    ./Working_code.o sample_test.txt ./Run_time_test_cases/sampleout_for_${j}_threads_for_batch2.txt $BOUND_BOX_DIMENSION 1 $j
done