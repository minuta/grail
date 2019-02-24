#!/usr/bin/env python3

# calculate mean, standard deviation and confidence interval from numbers given 
# as program params
#
# Usage:
#   calc.py <num-1> <num-2> ... <num-n>

import sys
import numpy as np
from scipy import stats
from math import sqrt


def get_numbers_from_cmd():
    numbers = [float(arg) for arg in sys.argv[1:]]
    num_of_items = len(sys.argv)-1

    if num_of_items == 0:
        print ("give a list of numbers as cmd params")
        sys.exit(1)
    return numbers


def calc (num_list):
    mean = np.mean(num_list)
    sd = np.std(num_list)
    variance = np.var(num_list)
    num_of_items = len(num_list)

    confidence = stats.norm.interval(0.95, loc=mean, scale=sd/sqrt(num_of_items))
    # see for details for a correct way to calculate confidence interval: 
    # https://stackoverflow.com/questions/28242593/correct-way-to-obtain-confidence-interval-with-scipy


    print ("- num of items        : {}".format(num_of_items))
    print ("- mean                : {}".format(mean))
    print ("- Standard deviation  : {}".format(sd))
    print ("- Variance            : {}".format(variance))
    print ("- Confidence interval : {}".format(confidence))

def main():
    numbers_list = get_numbers_from_cmd()
    calc(numbers_list)

  
if __name__== "__main__":
  main()
