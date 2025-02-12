#!/bin/bash
# count down
for i in {1..5}
do
    echo -n "$((6-i))..."
    sleep 1s
done
echo "Boom..."
read -p "Press any key to continue..."
