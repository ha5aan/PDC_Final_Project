#!/bin/bash
mpicc -o matrix mpiMult.c

mpiexec -np 11 ./matrix | tee answer.txt

gcc -o matomp -fopenmp openmpmulti.c
./matomp
./matomp >>answer.txt

gcc -o matseq sequentialmulti.c
./matseq
./matseq>>answer.txt
