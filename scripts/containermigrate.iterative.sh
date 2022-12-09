#!/bin/bash

#image="adyanth/cse291-test:nginx-mem-v1"
image="nginx:alpine"
port="80"
iters=3
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
echo Initial Checkpoint
docker checkpoint create --leave-running --checkpoint-dir /transfer --tcp-established --file-locks test migrate-part0
echo -e Done, size = `du -sh /transfer/migrate-part0 | cut -f1` "\n"
for i in $( seq 1 $iters )
do
  echo Checkpointing "$i"
  docker checkpoint create --leave-running --parent-path "../../transfer/migrate-part$((i-1))" --checkpoint-dir /transfer --tcp-established test "migrate-part${i}"
  echo -e Done, size = `du -sh "/transfer/migrate-part${i}" | cut -f1` "\n"
done
echo Final Checkpoint
mid=$(($(date +%s%N)/1000000))
sudo docker checkpoint create --parent-path "../../transfer/migrate-part${iters}" --checkpoint-dir=/transfer --tcp-established --file-locks test migrate-final
#sudo systemctl stop keepalived.service
sudo ssh "$target" 'docker start --file-locks --open-tcp --checkpoint migrate-final --checkpoint-dir=/transfer test'
end=$(($(date +%s%N)/1000000))
echo -e Done, size = `du -sh /transfer/migrate-final | cut -f1` "\n"
total=`du -sh "/transfer" | cut -f1`
echo "Migration complete in $(($end-$start))ms, total data ${total}, downtime $(($end-$mid))ms"
