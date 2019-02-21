#!/usr/bin/env python3

# calculate an average from numbers given as program params
# Usage:
#   calc.py <num-1> <num-2> ... <num-n>

import sys

numbers = [float(arg) for arg in sys.argv[1:]]

sum_of_numbers = sum(numbers)
num_of_items = len(sys.argv)-1

if num_of_items == 0:
    # raise NameError('give a list of numbers as cmd params!')
    print ("give a list of numbers as cmd params")
    sys.exit(1)

avarage = sum_of_numbers/float(num_of_items)

print ("num of items: {}".format(num_of_items))
print ("sum of items: {}".format(sum_of_numbers))
print ("average : {}".format(avarage))
