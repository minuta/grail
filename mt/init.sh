#!/bin/sh

# this script creates symbolic links in NS-3 scratch and NS-3 root folder,
# so that particulary WAF can notice presence of appropriate files in the multi-threading folder

OK_MSG="tput setaf 2; echo "OK"; tput sgr 0"
MULTI_THREADING_DIR=src/grail/mt
FILE_1=hl.sh
FILE_2=runner.cc

FILE_3=test-1.cc
FILE_4=test-2.cc
FILE_5=test-3.cc
FILE_6=test-4.cc

echo -n "---> changing to NS-3 root folder: "
cd ../../../ && eval $OK_MSG

echo -n "---> creating sym link for hl.sh : "
ln -sf $MULTI_THREADING_DIR/$FILE_1 $FILE_1 && eval $OK_MSG

echo -n "---> changing to scratch folder: "
cd scratch && eval $OK_MSG

echo -n "---> creating sym link for runner.cc : "
ln -sf ../$MULTI_THREADING_DIR/$FILE_2 $FILE_2 && eval $OK_MSG


# ------------------- TEST CASES ---------------------------------

echo -n "---> creating sym link for test-1.cc : "
ln -sf ../$MULTI_THREADING_DIR/$FILE_3 $FILE_3 && eval $OK_MSG

echo -n "---> creating sym link for test-2.cc : "
ln -sf ../$MULTI_THREADING_DIR/$FILE_4 $FILE_4 && eval $OK_MSG

echo -n "---> creating sym link for test-3.cc : "
ln -sf ../$MULTI_THREADING_DIR/$FILE_5 $FILE_5 && eval $OK_MSG

echo -n "---> creating sym link for test-4.cc : "
ln -sf ../$MULTI_THREADING_DIR/$FILE_6 $FILE_6 && eval $OK_MSG

# TODO: update echos with filename-variable
