#!/bin/bash
echo "parameters: number_of_vms"

ips[0]="192.168.122.38"
ips[1]="192.168.122.38"
ips[2]="192.168.122.69"
ips[3]="192.168.122.226"
ips[4]="192.168.122.92"
ips[5]="192.168.122.49"
ips[6]="192.168.122.241"
ips[7]="192.168.122.213"
ips[8]="192.168.122.36"
ips[9]="192.168.122.140"
ips[10]="192.168.122.7"

sshprefix="sshpass -p mihir scp mihir@"
readpath=":seq_read_1GB.out "
writepath=":seq_write_1GB.out "
copyloc="~/mnt/vm_tests/"

echo "Starting tests"
for (( c=1; c<=$1; c++))
do
    currip=${ips[$c]}
    filename="/seq_rd.out."
    cmd=$sshprefix$currip$readpath$copyloc$1$filename$c
    echo $cmd
    eval $cmd
    filename="/seq_wr.out."
    cmd=$sshprefix$currip$writepath$copyloc$1$filename$c
    echo $cmd
    eval $cmd
done
