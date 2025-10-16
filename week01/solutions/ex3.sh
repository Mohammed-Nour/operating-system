mkdir ~/week01/root
date
sleep 3
mkdir ~/week01/home
date
sleep 3
touch ~/week01/root/root.txt
date
sleep 3
touch ~/week01/home/home.txt
ls -tr / > ~/week01/root/root.txt
ls -tr ~/ > ~/week01/home/home.txt
ls ~/week01/root 
cat ~/week01/root/root.txt
ls ~/week01/home 
cat ~/week01/home/home.txt
