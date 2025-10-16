gcc worker.c -o worker
gcc scheduler.c -o scheduler
./scheduler "data.txt" 
rm worker
rm scheduler

