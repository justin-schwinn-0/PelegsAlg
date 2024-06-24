#!/bin/bash

# Change this to your netid
netid=jts200006

# Root directory of your project
PROJDIR=/home/justin/Desktop/code/CS6380/Pelegs

# Directory where the config file is located on your local system
CONFIGLOCAL=$PROJDIR/testConfig.txt

# Directory your java classes are in
BINDIR=$PROJDIR/build

# Your main project class
PROG=Pelegs

n=0

cat $CONFIGLOCAL | sed -e "s/#.*//" | sed -e "/^\s*$/d" |
(
    read i
    echo $i
    while [[ $n -lt $i ]]
    do
    	read line
    	p=$( echo $line | awk '{ print $1 }' )
        host=$( echo $line | awk '{ print $2 }' )
	
	gnome-terminal -- ssh $netid@$host "echo test"; exec bash &

        n=$(( n + 1 ))
    done
)
