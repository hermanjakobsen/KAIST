#!/bin/bash

# Compile & run
echo 'Compile and run controller simulation'
g++ -std=c++11 IT2FLS_HW.cpp main.cpp -o main.o
./main.o

# Plotting
echo 'Plotting...'
matlab -nosplash -nodesktop -r "run('plotting.m');exit" | tail -n +11