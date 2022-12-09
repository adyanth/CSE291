#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

#systemctl start keepalived.service
ip route del 172.31.11.2/32
ip address add dev eth0 172.31.11.2/20
rm -rf /home/admin/{a,b,c}
mkdir -p /home/admin/{a,b,c}

./test.py  &
PID=$!
read  -n 1 -p "Connect with a client: curl 172.31.11.2:8080 and hit enter" input
time criu pre-dump -D a -t $PID
scp -r a cse291-ahosavalike-deb2:~/
time criu pre-dump -D b --prev-images-dir ../a -t $PID
scp -r b cse291-ahosavalike-deb2:~/
time criu dump --shell-job --tcp-established -D c --prev-images-dir ../b --track-mem --leave-stopped -t $PID && kill -9 $PID
time scp -r c cse291-ahosavalike-deb2:~/
#systemctl stop keepalived.service
ip address del dev eth0 172.31.11.2/20
ip route add 172.31.11.2/32 via 172.31.11.34 dev eth0
