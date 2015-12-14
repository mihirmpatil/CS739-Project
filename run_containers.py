import subprocess
import os
import sys
import time

cnt = 1;
firstrun = 1;

launchcommand = 'python launch_containers.py testimage:v7 '
copycommand = 'python copy_container_files.py ~/mnt/container_tests/'
killcommand = 'python kill_containers.py all'

while (cnt <= 2):
    output = subprocess.check_output("ps -all", shell=True)
    output_list = output.strip().split('\n')
    processes = len(output)

    if (processes == 281):
        #Run the containers
        if firstrun != 1:
            #Copy the containers
            copycommand = 'python copy_container_files.py ~/mnt/container_tests/' + str(cnt)
            print copycommand
            output = subprocess.check_output(copycommand, shell=True);
            print output

            #Kill the containers
            print killcommand
            output = subprocess.check_output(killcommand, shell=True);
            print output
            cnt = cnt + 1
        else:
            firstrun = 0

        launchcommand = 'python launch_containers.py testimage:v7 ' + str(cnt)
        print launchcommand
        output = subprocess.check_output(launchcommand, shell=True);
        print output
        time.sleep(60)

    else:
        print 'Sleeping for 60 secs'
        time.sleep(60)
