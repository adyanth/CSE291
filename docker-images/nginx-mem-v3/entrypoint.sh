#!/bin/sh

python3 busy.py &
nginx -g "daemon off;"
