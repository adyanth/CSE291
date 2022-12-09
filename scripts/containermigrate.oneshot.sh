#!/bin/bash

#image="adyanth/cse291-test:nginx-mem-v3"
image="nginx:alpine"
port="80"
iters=0
target="root@172.31.1.2"

sudo rm -rf /shared/* /transfer/*
#sudo systemctl start keepalived.service
sudo docker rm -f test
sudo ssh "$target" "docker rm -f test"
sudo docker run -v /shared:/data -dp 8080:"${port}" --name test "${image}"
echo Press Enter to continue
read
sudo ssh "$target" "docker create -v /shared:/data -p 8080:${port} --name test ${image}"
start=$(($(date +%s%N)/1000000))
sudo docker checkpoint create --checkpoint-dir=/transfer --tcp-established --file-locks test migrate-final
sudo ssh "$target" 'docker start --file-locks --open-tcp --checkpoint migrate-final --checkpoint-dir=/transfer test'
end=$(($(date +%s%N)/1000000))
echo -e Done, size = `du -sh /transfer/migrate-final | cut -f1` "\n"
total=`du -sh "/transfer" | cut -f1`
echo "Migration complete in $(($end-$start))ms, total data ${total}"
