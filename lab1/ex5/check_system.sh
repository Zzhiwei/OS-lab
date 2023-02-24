#!/bin/bash

####################
# Lab 1 Exercise 5
# Name: 
# Student No: 
# Lab Group: 
####################

# Fill the below up
hostname=$HOSTNAME
machine_hardware=$OSTYPE
max_process_id=`cat /proc/sys/kernel/pid_max`

numlines=`ps | awk 'END{print NR}'`
user_process_count=$(($numlines-1))

user_with_most_processes=`ps -eo user|sort|uniq -c|sort -n|tail -1|awk '{print $2}'`
mem_free_percentage=`cat /proc/meminfo | awk '/MemFree/{free=$2} /MemTotal/{total=$2} END{print (free*100)/total}'`

echo "Hostname: $hostname"
echo "Machine Hardware: $machine_hardware"
echo "Max Process ID: $max_process_id"
echo "User Processes: $user_process_count"
echo "User With Most Processes: $user_with_most_processes"
echo "Memory Free (%): $mem_free_percentage"
