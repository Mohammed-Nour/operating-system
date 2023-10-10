#!/bin/bash
gcc ex2.c -pthread -o ex2
./ex2 $1
rm -f ex2
