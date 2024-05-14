#!/usr/bin/python

import sys
import random
import subprocess
import timeit
import os

def usage(arg1):
    print('Usage:', arg1, '-a|-s|-m|-d', '<size>', '<output file>')
    print('Run operation on args of size <size>', 'and output to file <output file>')

def get_random_number_file(size):
    rand_file = subprocess.run(["./gen_num", str(size)], encoding='utf-8', 
                                capture_output=True)
    file_name = rand_file.stdout.rstrip()
    f = open(file_name)
    num = f.read()
    return file_name, int(num)

def run_local(op, a1, a2):
    match op:
        case '-a':
            return a1 + a2;
        case '-s':
            return a1 - a2;
        case '-m':
            return a1 * a2;
        case '-d':
            return a1 // a2;


def run_and_compare(op, sz, out_file):
    f1,a1 = get_random_number_file(sz)
    f2,a2 = get_random_number_file(sz)

    print("Running the operation locally")
    local_result = run_local(op, a1, a2)
    print("Running the operation via b32")
    start_time = timeit.default_timer()
    result = subprocess.run(["./file_ops", op, f1, f2, out_file],
                                encoding='utf-8', capture_output=True)
    print("Time taken is ", timeit.default_timer() - start_time)
    print("Reading b32 result")
    
    f = open(out_file)
    file_num = f.read()
    b32_result = int(file_num)
    if local_result != b32_result:
        print('fail')
        print(a1)
        print(a2)
        print(b32_result)
        print(local_result)
        print('----------------------------------------')
    else:
        print('pass')

    
if __name__ == '__main__':
    sys.set_int_max_str_digits(1000000)
    if len(sys.argv) != 4:
        usage(sys.argv[0])
        exit(1)

    op = sys.argv[1]
    ops_set = ['-a', '-s', '-m', '-d']
    if (op not in ops_set):
        usage(sys.argv[0])
        exit(1)

    arg_size = int(sys.argv[2])
    run_and_compare(op, arg_size, sys.argv[3])
