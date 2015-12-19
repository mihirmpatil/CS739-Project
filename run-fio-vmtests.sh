#!/bin/bash
echo "parameters: number_of_vms"

ips[0]="192.168.122.38"
ips[1]="192.168.122.69"
ips[2]="192.168.122.226"
ips[3]="192.168.122.92"
ips[4]="192.168.122.49"
ips[5]="192.168.122.241"
ips[6]="192.168.122.213"
ips[7]="192.168.122.36"
ips[8]="192.168.122.140"
ips[9]="192.168.122.7"

sshprefix="(sshpass -p mihir ssh mihir@"
testcmd=' bash runfiotest.sh)'
compilecmd=" \"cd CS739-Project/disk && make clean && make\")"
runbg=" &"

echo "Started compiling binaries"
#Update git and compile the binaries
for (( c=0; c<$1; c++))
do
    currip=${ips[$c]}
    #update git
    gitcmd=" \"cd CS739-Project && git pull\")"
    ucmd=$sshprefix$currip$gitcmd
    eval $ucmd
    cmd=$sshprefix$currip$compilecmd
    echo $cmd
    eval $cmd
done
echo "Finished compiling binaries"

echo "Starting tests"
for (( c=0; c<$1; c++))
do
    currip=${ips[$c]}
    cmd=$sshprefix$currip$testcmd$runbg
    echo $cmd
    eval $cmd
done
