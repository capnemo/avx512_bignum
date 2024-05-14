#!/usr/bin/python

import sys
import random
import subprocess
import timeit

def usage(arg1):
    print("Usage:", arg1, '-a|-s|-m|-d|-r')
    print("Usage:", arg1, 'all')
    print("Usage:", arg1, 'sign')

def gen_random_number(size):
    random.seed()
    rand_str = str(random.randrange(1, 10, 1))
    for _ in range(size - 1):
        rand_str += str(random.randrange(10))

    return rand_str

def random_number_from_file(size):
    rand_file = subprocess.run(["./gen_num", str(size)], encoding='utf-8', 
                                capture_output=True)
    f = open(rand_file.stdout.rstrip())
    num = f.read()
    return num

class random_numbers:
    rnums = {}
    def __init__(self, max_sz):
        dic_sz = len(random_numbers.rnums)
        if (dic_sz < max_sz):
            print('Generating random numbers')
            for sz in range(dic_sz + 1, max_sz + 1):
                random_numbers.rnums[sz] = random_number_from_file(sz)

    @staticmethod
    def get_rand_number(sz):
        return random_numbers.rnums[sz]
            
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
        case '-r':
            return a1 % a2;

def run_and_compare(op, a1, a2):
    local_result = run_local(op, int(a1), int(a2))
    result = subprocess.run(["./operator_drv", op, a1, a2], 
                                encoding='utf-8', capture_output=True)
    
    b32_result = int(str(result.stdout).rstrip())
    if local_result != b32_result:
        print('fail')
        print(a1)
        print(a2)
        print(b32_result)
        print(local_result)
        print('----------------------------------------')

def run_op_for_sizes(op):
    arg_sizes = [1, 2, 3, 4, 8, 10, 20, 40, 100, 200, 1000]
    start_time = timeit.default_timer()
    rg = random_numbers(1000)

    for a in arg_sizes:
        arg1 = rg.get_rand_number(a)
        print('Running test for', op, 'for size', a)
        for s in range(1, a + 1):
            arg2 = rg.get_rand_number(s)
            run_and_compare(op, arg1, arg2)
            run_and_compare(op, arg2, arg1)
    print("Time taken for", op, "is", timeit.default_timer() - start_time)
    
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
    ops_set = ['-a', '-s', '-m', '-d', '-r']
    for op in ops_set:
        run_op_for_sizes(op)
    run_sign_tests()

def test_signs_for_size(sz):
    rg = random_numbers(2*sz)
    a1 = rg.get_rand_number(sz);
    a2 = rg.get_rand_number(sz);
    args_list = [a1, '-' + a1, a2, '-' + a2]
    ops_list = ['-a', '-s', '-m', '-d']
    
    for p in ops_list:
        for a in args_list:
            for b in args_list:
                run_and_compare(p, a, b)
                run_and_compare(p, b, a)
            
def run_sign_tests():
    print('Running sign tests')
    test_signs_for_size(3)
    test_signs_for_size(15)
    
if __name__ == '__main__':
    if len(sys.argv) > 3 or len(sys.argv) == 1:
        usage(sys.argv[0])
        exit(1)

    op = sys.argv[len(sys.argv) - 1]
    ops_set = ['-a', '-s', '-m', '-d', '-r', 'all', 'sign']
    if (op not in ops_set):
        usage(sys.argv[0])
        exit(1)

    if (sys.argv[1] == 'sign'):
        run_sign_tests()
        exit(0)

    if (sys.argv[1] == 'all'):
        run_all()
        exit(0)

    if (sys.argv[1] == '-x'):
        run_op_for_specifics(op)
    else:
        run_op_for_sizes(op)
