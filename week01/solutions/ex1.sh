#!/bin/bash

# Create the "week01" directory in the home directory
mkdir ~/week01

# Change to the "week01" directory
cd ~/week01

# List the last 5 entries in /usr/bin that contain "gcc" in reverse alphabetical order
find /usr/bin -type f -name '*gcc*' | sort -r | tail -n 5  > ~/week01/ex1.txt
