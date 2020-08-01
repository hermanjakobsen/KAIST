#!/bin/bash

# Compile & run GA
echo 'Compile and run GA'
gcc GA/GA_Dejong.c -o ga.o -lm
./ga.o

# Compile & run QEA
echo 'Compile and run QEA'
g++ -std=c++11 QEA/HStopwatch.cpp QEA/QEAtest.cpp -o qea.o
./qea.o