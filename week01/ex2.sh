# Create a directory named "test"
mkdir test

# Create another directory named "test2"
mkdir test2

# Change to the "test2" directory
cd test2

# Ping the IP address "192.168.1.1" five times and append the output to a file named "logs.txt"
ping -c 5 192.168.1.1 >> logs.txt

# Display the contents of "logs.txt"
cat logs.txt

# Copy "logs.txt" to the parent directory (one level up)
cp logs.txt ..

# Move back to the parent directory
cd ..

# Remove the "test" directory
rm -r test

# Remove the "test2" directory
rm -r test2

# Remove the "logs.txt" file (assuming you want to delete it)
rm logs.txt

# List the contents of the current directory
ls

# Print the current working directory
pwd

# Display the username of the current user
whoami

# Print "done" to the console
echo done

# Save the command history to a file named "ex2.txt"
history > ex2.txt

# Save the last 15 commands from the history to "ex2.txt"
history | tail -n 15 > ex2.txt
