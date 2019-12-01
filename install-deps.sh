#!/bin/bash

apt update
apt install runc jq


curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh

sudo usermod -aG docker $USER

apt install -y gcc

export RIDDLER_SHA256="cf319c1be2f6007736c6a618d0746dcff4f291fc112c9d7150c5270b4c182580"
curl -fSL "https://github.com/genuinetools/riddler/releases/download/v0.6.3/riddler-linux-amd64" -o "/usr/local/bin/riddler" \
	&& echo "${RIDDLER_SHA256}  /usr/local/bin/riddler" | sha256sum -c - \
	&& chmod a+x "/usr/local/bin/riddler"

# rm -r rootfs 
# mkdir rootfs
# docker export $(docker create busybox) | tar -C rootfs -xvf -
# rm config.json
# runc spec

