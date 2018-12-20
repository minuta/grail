#!/usr/bin/env bash

# this script tests reproducibility of Grail in multi-threaded mode

OK_MSG="tput setaf 10; echo ' Success!'; tput sgr 0"
FAILED_MSG="tput setaf 9; echo ' FAILED!'; tput sgr 0"
#OK_MSG="tput setaf 2; echo "OK"; tput sgr 0"
MSG="capturing iteration in :"
CAPTURE_FILE_1=repr-capture1
CAPTURE_FILE_2=repr-capture2
TEST_SCRIPT=test-3

echo "$MSG" $CAPTURE_FILE_1 && LD_LIBRARY_PATH=build:../../../build/lib ../../../build/src/grail/mt/ns3-dev-runner-debug --app=./$TEST_SCRIPT > $CAPTURE_FILE_1 || eval $FAILED_MSG 
echo "$MSG" $CAPTURE_FILE_2 && LD_LIBRARY_PATH=build:../../../build/lib ../../../build/src/grail/mt/ns3-dev-runner-debug --app=./$TEST_SCRIPT > $CAPTURE_FILE_2 || eval $FAILED_MSG

# if capture file empty then exit
[[ ! -s $CAPTURE_FILE_1 ]] && rm $CAPTURE_FILE_1 $CAPTURE_FILE_2 && exit 1

echo -n "comparing generated captures :"
diff -q $CAPTURE_FILE_1 $CAPTURE_FILE_2 && eval "$OK_MSG" || eval "$FAILED_MSG"

rm $CAPTURE_FILE_1 $CAPTURE_FILE_2
