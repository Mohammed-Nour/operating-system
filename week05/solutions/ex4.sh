#!/bin/bash

gcc ex4.c -pthread -o ex4.out
for m in 1 2 4 10 100; do
    echo -n "answer for n = 10000000, m = $m is: "
    time ./ex4.out 10000000 $m
done
echo 'cleaning...'
rm ex4.out