# calculate an average from numbers given as program params

import sys

numbers = [float(arg) for arg in sys.argv[1:]]

sum_of_numbers = sum(numbers)
num_of_items = len(sys.argv)-1

avarage = sum_of_numbers/float(num_of_items)

print ("num of items: {}".format(num_of_items))
print ("sum of items: {}".format(sum_of_numbers))
print ("average : {}".format(avarage))
