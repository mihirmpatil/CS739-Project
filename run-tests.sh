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


echo "Start: " $(date +%s)
#cleanup
#setup
#cd CS739-Project-master
cd disk
make

./seq_write file.1GB 262144 > seq_write_1GB.out
sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
./seq_read file.1GB > seq_read_1GB.out
echo "End: " $(date +%s)
