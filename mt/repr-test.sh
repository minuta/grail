#!/usr/bin/env bash

# this script is useful for testing simulation's reproducibility
# it runs a given binary executable as a node application in NS-3 and compares generated stdout
# The binary will automatically configured and started through the NS-3 application runner.
# (see runner.cc)
# 
# SYNOPSIS:
#           <repr-test.sh> <binary to run> <number of iterations>
# 

OK_MSG="tput setaf 10; echo ' Success!'; tput sgr 0"
FAILED_MSG="tput setaf 9; echo ' FAILED!'; tput sgr 0"
#OK_MSG="tput setaf 2; echo "OK"; tput sgr 0"
#MSG="capturing iteration in :"
CAPTURE_FILE_1=repr-capture1
CAPTURE_FILE_2=repr-capture2
TEST_SCRIPT=$1
NUM_OF_ITERATIONS=$2
i=1

if [ -z "$1" ]
  then
      echo "Error : an argument specifying binary (to execute on the runner app) was not given"
      exit 1
fi

if [ -z "$2" ]
  then
      echo "Warning : an argument specifying number of iterations was not given, thus take a default value = 1"
      NUM_OF_ITERATIONS=1
fi

echo
echo "testing binary $TEST_SCRIPT with test-runner for $NUM_OF_ITERATIONS iterations"
echo

while [ "$i" -le "$NUM_OF_ITERATIONS" ]; do
    echo Running iteration "$i" ---------------------------
    #i=$(($i + 1))

    echo - capturing stdout for the first execution && LD_LIBRARY_PATH=build:../../../build/lib ../../../build/src/grail/mt/ns3-dev-runner-debug --app=./$TEST_SCRIPT > $CAPTURE_FILE_1 || eval $FAILED_MSG 
    echo - capturing stdout for the second execution && LD_LIBRARY_PATH=build:../../../build/lib ../../../build/src/grail/mt/ns3-dev-runner-debug --app=./$TEST_SCRIPT > $CAPTURE_FILE_2 || eval $FAILED_MSG

    # if capture file empty then exit
    [[ ! -s $CAPTURE_FILE_1 ]] && rm $CAPTURE_FILE_1 $CAPTURE_FILE_2 && exit 1

    echo -n "- comparing generated captures :"
    diff -q $CAPTURE_FILE_1 $CAPTURE_FILE_2 && eval "$OK_MSG" || eval "$FAILED_MSG"

    rm $CAPTURE_FILE_1 $CAPTURE_FILE_2
    echo
done


