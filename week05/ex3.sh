#!/bin/bash

gcc ex3.c -pthread -o ex3.out
for m in 1 2 4 10 100; do
    echo -n "answer for n = 10000000, m = $m is: "
    time ./ex3.out 10000000 $m
done
echo 'cleaning...'
rm ex3.out