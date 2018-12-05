#!/bin/sh

# this script works with NS-3 logs and has two tasks: 
#  1) remove all log lines until the first match
#  2) highlights tags/keywords with given colors in the remaining log lines
#
# USAGE:
# 
#  without highlighting TIDs:   ./hl.sh  <filename> 
#  with highlighting TIDs :     ./hl.sh  <filename> <TID-1> <TID-2>
# 

TAG_1="THREAD #1"
TAG_2="THREAD #2"
TAG_3="clone"
TAG_4=$2      # TID 1
TAG_5=$3      # TID 2

COLOR_TAG_1="bright_green" 
COLOR_TAG_2="bright_yellow"
COLOR_TAG_3="red"
COLOR_TAG_4="blue"
COLOR_TAG_5="bright_cyan"

cat "$1" | sed -n '/THREAD/,$p'\
        | ack -i --flush --passthru --color --color-match="$COLOR_TAG_1" "$TAG_1"\
        | ack -i --flush --passthru --color --color-match="$COLOR_TAG_2" "$TAG_2"\
        | ack -i --flush --passthru --color --color-match="$COLOR_TAG_3" "$TAG_3"\
        | ack -i --flush --passthru --color --color-match="$COLOR_TAG_4" "$TAG_4"\
        | ack -i --flush --passthru --color --color-match="$COLOR_TAG_5" "$TAG_5"
