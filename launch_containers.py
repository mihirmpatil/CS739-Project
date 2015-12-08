import os
import sys
import subprocess

image_name = sys.argv[1]
count = int(sys.argv[2])
privileged_str = "--privileged"
mem_str = "-m 4g"
cmd = "sudo docker run -d -it {0} {1} {2} /bin/bash".format(privileged_str, mem_str, image_name)
for i in range(count):
	print cmd
	output = subprocess.check_output(cmd, shell=True)
	print output
