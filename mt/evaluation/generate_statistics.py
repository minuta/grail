#!/usr/bin/env python3


import sys
import numpy as np
from scipy import stats
from math import sqrt


def get_filename_from_cmd():
    num_of_items = len(sys.argv)-1

    if num_of_items == 0:
        print ("Error: no filename given...")
        print ("give a filename with data of the following form:\n")
        print ("name1   name2   name3   name4 ")
        print ("1       5       4.5     4.7")
        print ("100     5       5.1     5.3")
        print ("200     5       5.3     4.5")
        sys.exit(1)
    return sys.argv[1]


def make_sum(list):
    float_list = [float(i) for i in list]
    return float_list

def print_old_table(filename):
    f = open(filename, "r")
    print ("-"*80)
    print ("Original data table:\n")
    for line in f:
        line = line.strip('\n')
        print (line)
    print ("-"*80)
    f.close()

def count_mean_from_list(float_list):
    return np.mean(float_list)


def print_new_table(filename):

    f = open(filename, "r+")
    print ("-"*80)
    print ("New data table:\n")

    cnt = 0

    for line in f:
        cnt+=1
        if (cnt == 1):    # ignore headline
           print (line.strip('\n') + "Mean" + "\t\t" + "CI-Lo" + "\t\t" + "CI-Hi")
           continue
        list_from_line = line.split()
        float_list = [float(i) for i in list_from_line]
        line = line.strip('\n')

        numbers = float_list[1:]
        mean = np.mean(numbers)
        sd = np.std(numbers)
        confidence = stats.norm.interval(0.95, loc=mean, scale=sd/sqrt(len(numbers)))
        ci_lo, ci_hi = confidence

        line += "\t" + str(np.mean(numbers)) + "\t" + str(ci_lo) + "\t" + str(ci_hi)

        print (line)
        # print (confidence)

    print ("-"*80)
    f.close()

# -----------------------------------------------------------------------------
def main():

    filename = get_filename_from_cmd()
    print ("Calculating data from ", filename)
    print_old_table(filename)
    print_new_table(filename)


if __name__== "__main__":
    main()
