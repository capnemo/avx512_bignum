#!/usr/bin/python

import sys
import random
import subprocess

def usage(arg1):
    print("Usage:", arg1, "-a|-s|-m|-d")

def gen_random_number(size):
    random.seed()
    rand_str = str(random.randrange(1, 10, 1))
    for _ in range(size - 1):
        rand_str += str(random.randrange(10))

    return rand_str

def random_number_from_file(size):
    rand_file = subprocess.run(["./gen_num", str(size)], encoding='utf-8', 
                                capture_output=True)
    #print(rand_file.stdout.rstrip())
    f = open(rand_file.stdout.rstrip())
    num = f.read()
    return num

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

def run_and_compare(op, a1, a2):
    local_result = run_local(op, int(a1), int(a2))
    result = subprocess.run(["./operator_drv", op, a1, a2], 
                                encoding='utf-8', capture_output=True)
    b32_result = int(str(result.stdout).rstrip())
    if local_result != int(b32_result):
        print('fail')
        print(a1)
        print(a2)
        print(b32_result)
        print(local_result)
        print('----------------------------------------')

def run_op_for_sizes(op):
    arg_sizes = [1, 2, 3, 4, 8, 10, 20, 40, 100, 200, 1000]
    for a in arg_sizes:
        arg1 = random_number_from_file(a)
        print('Running test for', op, 'for size', a)
        for s in range(1, a + 1):
            arg2 = random_number_from_file(s)
            run_and_compare(op, arg1, arg2)
            run_and_compare(op, arg2, arg1)
    
def run_op_for_specifics(op):
    b32_digit = 2^32

    b32_large = 0
    for i in range(1, 20):
        b32_large += i*(b32_digit**i)

    b32_mid = 0
    for i in range(1, 5):
        b32_mid += i*(b32_digit**i)
    
    left_args = [b32_digit - 1, b32_digit, b32_digit + 1, b32_mid, b32_large]
    right_args = [1, 2, b32_mid, b32_digit]
    
    print("Running specific tests for ", op)
    for l in left_args:
        for r in right_args:
            lstr = str(l)
            rstr = str(r)
            run_and_compare(op, lstr, rstr)
            run_and_compare(op, rstr, lstr)

def run_all():
    ops_set = ['-a', '-s', '-m', '-d']
    for op in ops_set:
        run_op_for_sizes(op)
        

if __name__ == '__main__':
    if len(sys.argv) > 3 or len(sys.argv) == 1:
        usage(sys.argv[0])
        exit(1)

    op = sys.argv[len(sys.argv) - 1]
    ops_set = ['-a', '-s', '-m', '-d', 'all']
    if (op not in ops_set):
        usage(sys.argv[0])
        exit(1)

    if (sys.argv[1] == 'all'):
        run_all()
        exit(0)

    if (sys.argv[1] == '-x'):
        run_op_for_specifics(op)
    else:
        run_op_for_sizes(op)
