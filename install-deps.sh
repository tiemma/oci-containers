#!/bin/bash

apt update
apt install runc jq


curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh

sudo usermod -aG docker $USER


docker export $(docker create busybox) | tar -C rootfs -xvf -
runc spec

