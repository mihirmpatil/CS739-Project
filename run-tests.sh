#! /bin/bash

cleanup () {
cd ~
rm -f master.zip
rm -rf CS739-Project-master
}

setup () {
cd ~
wget https://github.com/mihirmpatil/CS739-Project/archive/master.zip
unzip master.zip
}


#cleanup
#setup
#cd CS739-Project-master

disk() {
cd disk
make

./seq_write file.1GB 262144 > seq_write_1GB.out
sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
./seq_read file.1GB > seq_read_1GB.out
}


memory() {
#cd memory
gcc CS739-Project/memory/contmemtest.c -pthread -o CS739-Project/memory/contmemtest
CS739-Project/memory/contmemtest 1 1024 4096 1024 1 1 > CS739-Project/memory/contmemtest.2c.4GB.out
}

echo "Start: " $(date +%s)
memory
echo "End: " $(date +%s)
