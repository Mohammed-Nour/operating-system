# Create a new directory named "root"
mkdir root

# Print the current date
date

# Wait for 3 seconds
sleep 3

# Create a new directory named "home"
mkdir home

# Print the current date
date

# Wait for 3 seconds
sleep 3

# Create an empty file named "root.txt" in the "root" directory
touch root/root.txt

# Print the current date
date

# Wait for 3 seconds
sleep 3

# List the items in the root directory '/' and save the results in "root.txt"
# The '-la' option lists all items, including hidden files, and '-tr' sorts them by time (oldest first)
ls -la -tr / > root/root.txt

# Create an empty file named "home.txt" in the "home" directory
touch home/home.txt

# Print the current date
date

# Wait for 3 seconds
sleep 3

# List the items in the home directory '~' (your home directory) and save the results in "home.txt"
# The '-la' option lists all items, including hidden files, and '-tr' sorts them by time (oldest first)
ls -la -tr ~ > home/home.txt

