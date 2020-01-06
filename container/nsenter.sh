#!/bin/bash

run_command_ns(){
    nsenter -t $PID -G $GROUP_ID -S $USER_ID -a "$1"
}


USER_ID=0
GROUP_ID=0
PID=$(ps -a | grep "\ssh$" | head -n1 | awk '{split($0,a," "); print a[1]}')

echo "Viewing the namespaces of the container, remember that containers are processes"
ls -l "/proc/$PID/ns"

echo "Enter nsenter for container with PID $PID"
# echo "65534 0 4294967295" > /proc/$PID/uid_map
# echo "65534 0 4294967295" > /proc/$PID/gid_map

run_command_ns "$1"