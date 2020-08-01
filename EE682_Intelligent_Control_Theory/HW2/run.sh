#!/bin/bash

# Compile & run
echo 'Compile and run controller simulation'
g++ -std=c++11 main.cpp -o main.o
./main.o

# Plotting
echo 'Plotting and calculating characteristics'
matlab -nosplash -nodesktop -r "run('plotting.m');exit" | tail -n +11