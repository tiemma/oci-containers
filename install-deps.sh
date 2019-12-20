#!/bin/bash

sudo apt update
sudo apt install runc jq gcc git

curl -fsSL https://get.docker.com -o get-docker.sh
sh get-docker.sh

sudo usermod -aG docker $USER

export RIDDLER_SHA256="cf319c1be2f6007736c6a618d0746dcff4f291fc112c9d7150c5270b4c182580"
curl -fSL "https://github.com/genuinetools/riddler/releases/download/v0.6.3/riddler-linux-amd64" -o "/usr/local/bin/riddler" \
	&& echo "${RIDDLER_SHA256}  /usr/local/bin/riddler" | sha256sum -c - \
	&& chmod a+x "/usr/local/bin/riddler"

git clone https://github.com/Tiemma/oci-containers 

cd oci-containers/chroot

gcc main.c -o chroot

sudo ./chroot image 

