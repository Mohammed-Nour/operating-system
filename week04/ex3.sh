#!/bin/bash

gcc ex3.c -o ex3

echo "Running with n=3"

./ex3 3 &

sleep  10

echo "PSTree output for n=3:"

pstree -p $! 

echo "Running with n=5"

./ex3 5 &

sleep 20

echo "PSTree output for n=5:"

pstree -p $!

rm ex3

