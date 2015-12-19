
import os
import sys
import subprocess

testparam="/root/CS739-Project-master/disk/seq_write /file.1GB 262144 > /seq_write_1GB.out && sudo sh -c \"sync; echo 3 > /proc/sys/vm/drop_caches\" && /root/CS739-Project-master/disk/seq_read /file.1GB > /seq_read_1GB.out"

def get_running_containers():
	list_cmd = "sudo docker ps -q"
	output = subprocess.check_output(list_cmd, shell=True)
	containers_list = output.decode("utf-8").strip().split('\n')
	return containers_list


def run_tests(container_name):
        cmd="(sudo docker exec -d {0} bash -c '{1}')".format(container_name,testparam)
        output = subprocess.check_output(cmd, shell=True)
        print cmd


running = get_running_containers()
for item in running:
	print item
	run_tests(item)

