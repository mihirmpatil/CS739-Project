
import os
import sys
import subprocess


file_path = "/root/CS739-Project-master/disk/"

def get_running_containers():
	list_cmd = "sudo docker ps -q"
	output = subprocess.check_output(list_cmd, shell=True)
	containers_list = output.decode("utf-8").strip().split('\n')
	return containers_list


def copy_files(container_name, file_names, dest_dir):
	global file_path
	for item in file_names:
		copy_cmd = "sudo docker cp {0}:{1}{2} {3}/{4}_{5}".format(container_name, file_path, item, dest_dir, container_name, item)
		output = subprocess.check_output(copy_cmd, shell=True)


if len(sys.argv) != 2:
	print "requires a destination directory"
	sys.exit()

dest_dir = sys.argv[1]
if os.path.isdir(dest_dir) is False:
	os.makedirs(dest_dir)


file_names_list = [ "seq_write_1GB.out",
										"seq_read_1GB.out"
									]
#file_paths = []
#for item in file_names_list:
#	file_paths.append("{0}{1}".format(file_path, item))


running = get_running_containers()
for item in running:
	print item
	copy_files(item, file_names_list, dest_dir)

