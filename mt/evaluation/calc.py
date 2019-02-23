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

numbers = [float(arg) for arg in sys.argv[1:]]
num_of_items = len(sys.argv)-1

# numbers = [110, 112, 106, 90, 96, 118, 108, 114, 107, 90, 85, 84, 113, 105, 90, 104]
# num_of_items = 16

if num_of_items == 0:
    print ("give a list of numbers as cmd params")
    sys.exit(1)

mean = np.mean(numbers)
sd = np.std(numbers)
variance = np.var(numbers)

confidence = stats.norm.interval(0.95, loc=mean, scale=sd/sqrt(num_of_items))
# see for details for a correct way to calculate confidence interval: 
# https://stackoverflow.com/questions/28242593/correct-way-to-obtain-confidence-interval-with-scipy


print ("- num of items        : {}".format(num_of_items))
print ("- mean                : {}".format(mean))
print ("- Standard deviation  : {}".format(sd))
print ("- Variance            : {}".format(variance))
print ("- Confidence interval : {}".format(confidence))
