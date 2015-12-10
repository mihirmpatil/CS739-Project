
import os
import sys
import subprocess


def get_running_containers():
	list_cmd = "sudo docker ps -q"
	output = subprocess.check_output(list_cmd, shell=True)
	containers_list = output.decode("utf-8").strip().split('\n')
	return containers_list

	
def kill_single_container(container_name):
	kill_cmd = "sudo docker rm -f {0}".format(container_name)
	output = subprocess.check_output(kill_cmd, shell=True)

def kill_all_containers():
	running = get_running_containers()
	for item in running:
		print item
		kill_single_container(item)


input_arg = sys.argv[1]
if input_arg == 'all':
	kill_all_containers()
else:
	kill_single_container(input_arg)
