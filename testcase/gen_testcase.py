#!/usr/bin/python3

#########################################################################
# Generate M x N matrix of real numbers and store                       #
# the the matrix in file named 'testcase_<M>_<N>'                       #
# Parameters:                                                           #
#   M               :no of rows (samples) in matrix                     #
#   N               :no of coulmns (features) in matrix                 #
#   lrange, urange  :range of matrix elements ie                        #
#                       forall  0<=i<M, 0<=j<N                          #
#                       lrange <= matrix[i][j] <= urange                #
# Format of output file:                                                #
#   -----------------------------------------------------------------   #
#   | M N                                                               #
#	| D[0][0] D[0][1] ... D[0][N-1] D[1][0] ... D[M-1][N-1]             #
#   -----------------------------------------------------------------   #
#########################################################################

import random
import string

def randomString(stringLength):
    """Generate a random string of fixed length """
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))

n = 100
num_patterns = 2
m_set = []
p_set = []
pattern_set = []

filename = 'testcase_' + str(n) + '_' + str(num_patterns)     #output filename
file = open(filename, 'w')

# write size of length of text and number of patterns in first line of file
file.write(str(n) + ' ' +str(num_patterns) + '\n')

# generate and write text
file.write(randomString(n) + '\n')

# generate and write length of patterns
for i in range(num_patterns):
	m = random.randint(4, n)
	m_set.append(m)
	file.write(str(m) + ' ')
file.write('\n')

# generate and write period length of patterns
for i in range(num_patterns):
	p = random.randint(2, m_set[i]/2)
	p_set.append(p)
	file.write(str(p) + ' ')
file.write('\n')

# generate and write patterns
for i in range(num_patterns):
	period_str = randomString(p_set[i])
	pattern = period_str * (m_set[i] / p_set[i])
	pattern = pattern + period_str[0:m_set[i]-len(pattern)]
	file.write(pattern + '\n')

file.close()