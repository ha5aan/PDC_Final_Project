#!/bin/bash
mpicc -o matrix mpiMult.c

mpiexec -np 4 ./matrix | tee answer.txt
