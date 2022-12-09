#!/bin/sh

python3 -c 'a = [0]*10000000; import time; time.sleep(100000)' &
nginx -g "daemon off;"
