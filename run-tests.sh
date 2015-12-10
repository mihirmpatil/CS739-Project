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
cd memory
gcc contmemtest.c -pthread -o contmemtest
./contmemtest 1 0 4096 64 1 1 > contmemtest.out
}

echo "Start: " $(date +%s)
memory
echo "End: " $(date +%s)
