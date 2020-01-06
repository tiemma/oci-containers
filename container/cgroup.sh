#!/bin/bash

ALPINE_DIR=

def_alpine_dir(){
    ALPINE_DIR="/sys/fs/cgroup/$1/alpine"
    mkdir $ALPINE_DIR
    echo $PID > ${ALPINE_DIR}/cgroup.procs
}

echo_nl(){
    echo -e "\n$1\n"
}


PID=$(ps -a | grep chroot | head -n1 | awk '{split($0,a," "); print a[1]}')
echo_nl "Viewing the namespaces of the container, remember that containers are processes"
ls -l "/proc/$PID/ns"

echo_nl "Create a memory namespace for the application"
def_alpine_dir memory
cat ${ALPINE_DIR}/memory.usage_in_bytes

echo_nl "Let's see the process before we set the memory limit low"
ps -o cgroup $PID

echo_nl "Let's set the memory limit to 4KB"
echo 500 > ${ALPINE_DIR}/memory.limit_in_bytes
cat ${ALPINE_DIR}/memory.limit_in_bytes


echo_nl "Let's restrict cpus to our process with cgroups"
cat /proc/$PID/status | grep Cpus_allowed

echo_nl "Let's assign cpus to our process"
def_alpine_dir cpuset
echo 0-0 > ${ALPINE_DIR}/cpuset.cpus


echo_nl "Let's see the process details after limiting cpu"
cat /proc/$PID/status | grep Cpus_allowed



#bash nsenter.sh "chmod +x while.sh"
#bash nsenter.sh "./while.sh"

echo_nl "Let's see the process again"
ps -o cgroup $PID