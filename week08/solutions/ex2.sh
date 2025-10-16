#!/bin/bash

# Compile the C programs
gcc -o pager pager.c
gcc -o mmu mmu.c

mkdir /tmp/ex2
# Start the pager process in the background
# Check if total frames is greater than total pages
total_pages=4
total_frames=2

if [ $total_frames -gt $total_pages ]; then
  echo "Error: Number of frames cannot be greater than number of pages."
  exit 1
fi


./pager $total_pages $total_frames &

# Get the PID of the pager process
pager_pid=$!

# Give some time for the pager process to start
sleep 1

# Start the MMU process with the specified arguments
./mmu $total_pages R0 R1 W1 R0 R2 W2 R0 R3 W2 $pager_pid


# Clean up (you can remove the binary files if you want)
rm pager mmu
