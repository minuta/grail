#!/usr/bin/env python3


import numpy as np
from scipy import stats
from math import sqrt


FILENAME = "data-1.dat"

# from math import sum

def make_sum(list):
    float_list = [float(i) for i in list]
    return float_list

def print_old_table():
    f = open(FILENAME, "r+")
    print ("-"*80)
    print ("Original data table:\n")
    for line in f:
        line = line.strip('\n')
        print (line)
    print ("-"*80)
    f.close()

def count_mean_from_list(float_list):
    return np.mean(float_list)


def print_new_table():

    f = open(FILENAME, "r+")
    print ("-"*80)
    print ("New data table:\n")

    cnt = 0

    for line in f:
        cnt+=1
        if (cnt == 1):    # ignore headline
           print line.strip('\n')
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
    print_old_table()
    print_new_table()


if __name__== "__main__":
    main()
