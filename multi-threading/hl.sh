#!/bin/sh

# USAGE:
# 
#  whithout highlightning TIDs:   ./hl.sh  <filename> 
#  with highlightning TIDs :      ./hl.sh  <filename> <TID-1> <TID-2>
# 

TAG_1="THREAD #1"
TAG_2="THREAD #2"
TAG_3="clone"
TAG_4=$2      # TID 1
TAG_5=$3      # TID 2

COLOR_TAG_1="green"
COLOR_TAG_2="yellow"
COLOR_TAG_3="red"
COLOR_TAG_4="blue"
COLOR_TAG_5="cyan"

# remove as first step all lines till first match = "THREAD"
cat "$1" | sed -n '/THREAD/,$p'\
        | ack --flush --passthru --color --color-match="$COLOR_TAG_1" "$TAG_1"\
        | ack --flush --passthru --color --color-match="$COLOR_TAG_2" "$TAG_2"\
        | ack -i --flush --passthru --color --color-match="$COLOR_TAG_3" "$TAG_3"\
        | ack --flush --passthru --color --color-match="$COLOR_TAG_4" "$TAG_4"\
        | ack --flush --passthru --color --color-match="$COLOR_TAG_5" "$TAG_5"
