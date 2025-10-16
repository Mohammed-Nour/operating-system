#!/bin/bash

# Ensure the PID file exists
if [[ ! -f /tmp/ex1.pid ]]; then
    echo "PID file does not exist."
    exit 1
fi

# Read the PID from /tmp/ex1.pid
pid=$(cat /tmp/ex1.pid)

# Check if the PID could be read
if [[ -z "$pid" ]]; then
    echo "Could not read PID from /tmp/ex1.pid."
    exit 1
fi

# Check for root permissions
if (( $EUID != 0 )); then
    echo "This script must be run as root."
    exit 1
fi

# Identify the memory mapping for shared anonymous mappings
maps_line=$(grep 'rw-s' /proc/$pid/maps | head -n 1)

# If there is no memory segment with rw-s permissions, we cannot find the password
if [[ -z "$maps_line" ]]; then
    echo "No suitable memory segment found."
    exit 1
fi

# Extract start and end addresses from the memory segment
start_addr=$(echo $maps_line | cut -d'-' -f1)

# Convert start address to decimal for xxd
start_addr_dec=$((16#$start_addr))

# Use xxd to convert memory contents starting from the mapping address, and grep to search for "pass:"
# Output format of xxd is: offset: hex bytes ascii
# We use awk to find the line containing "pass:" and to get the offset
password_line=$(sudo xxd -s $start_addr_dec -e /proc/$pid/mem | grep -oa 'pass:[[:print:]]\{8\}' | head -n 1)

if [[ -n $password_line ]]; then
    # Extract the password
    password=$(echo $password_line | cut -d' ' -f2)
    echo "Password: $password"

    # Calculate the actual memory address of the password
    # The following awk command parses the output of grep to get the byte offset, which we add to the start address
    offset=$(echo $password_line | grep -oba 'pass:' | head -n 1 | awk -F: '{print $1}')
    actual_address=$((start_addr_dec + offset))
    printf "Memory Address: 0x%x\n" $actual_address
else
    echo "Password not found."
    exit 1
fi

# Send SIGKILL to the program ex1.c
sudo kill -9 "$pid"
echo "Process $pid has been killed."
