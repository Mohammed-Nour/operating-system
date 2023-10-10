#!/bin/bash

[[ $1 > 3 ]] && echo "first arg must be smaller than 3" && exit 1
[[ $1 < 1 ]] && echo "first arg must be bigger than 1" && exit 1
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber
for ((i = 0; i < $1; i++)) do
 	gnome-terminal -- ./subscriber $i &
done
gnome-terminal -- ./publisher $1
