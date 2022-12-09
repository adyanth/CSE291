#!/bin/bash

sudo systemctl stop containerd docker

sudo pkill -f containerd-shim-runc-v2

sudo cp bak/* /usr/local/bin/

sudo cp bak/* /usr/bin

sudo systemctl start containerd docker
