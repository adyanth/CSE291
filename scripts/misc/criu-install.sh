#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

apt install --no-install-recommends -y \
    build-essential libprotobuf-dev libprotobuf-c-dev \
    protobuf-c-compiler protobuf-compiler python3-protobuf \
    pkg-config libbsd-dev iproute2 libnftables-dev libcap-dev \
    libnl-3-dev libnet-dev libgnutls28-dev asciidoc xmlto

wget https://github.com/checkpoint-restore/criu/archive/refs/tags/v3.17.1.tar.gz
tar -xvf v3.17.1.tar.gz
cd criu-3.17.1*
make clean
make -j 2
make install
criu check

