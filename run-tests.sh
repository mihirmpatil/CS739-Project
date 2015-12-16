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
cd /root/CS739-Project-master/disk
make clean
make
cd /

#./seq_write file.1GB 262144 > seq_write_1GB.out
sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
#./seq_read file.1GB > seq_read_1GB.out
}


memory() {
#cd memory
gcc CS739-Project/memory/contmemtest.c -pthread -o CS739-Project/memory/contmemtest
CS739-Project/memory/contmemtest 1 1024 4096 1024 1 1 > CS739-Project/memory/contmemtest.2c.4GB.out
}

cpu() {
#cd memory
gcc /root/CS739-Project-master/cpu/borwein_pi.c -pthread -o /root/CS739-Project-master/cpu/borwein
/root/CS739-Project-master/cpu/borwein 100000000 6 > /root/CS739-Project-master/cpu/cputest.100000000digit.out
}

echo "Start: " $(date +%s)
disk
echo "End: " $(date +%s)
