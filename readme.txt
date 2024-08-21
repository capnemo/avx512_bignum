
avx512_bignum is a fast, infinite precision library built around the avx512
extensions to the x86 instruction set. 

Prerequisites
-------------
a) Hardware that supports avx512 running a recent linux distribution.
b) g++ version >= 13.2.1
c) python version >= 3.12.3
b) and c) are what was used to develop this library. Older versions may work.

Building the library.
---------------------
Clone the software to your machine. 
Run make debug|release|fast
debug runs the compiler with the -ggdb flag
release runs the compiler with the -Os flag
fast runs the compiler with the -Ofast flag.
The above will build the library b32.a and all the test binaries.

API
---
The library implements 2^32 base arithmetic in the form of a b32 class.
The header is incl/b32.h. The four arithmetic operations are
a) add_to
b) subtract_from
c) multiply_with
d) divide_by
For a better understanding of the API, see the cpp files in the tests directory

Tests
-----
Regression tests.
-----------------
operator_drv will run an operation for a particular pair of operands.
run_all.py will run regression tests on any or all of the operations for a set of randomly generated operands.

Performance tests.
------------------
file_ops will run and time an operation on a pair of numbers contained in files.
perf_test.py will generate operands of a given size and run and time an operation on those numbers.
All commands when run without arguments will print usage instructions.

Current Status
----------------
References to the __m512i datatype need to be removed for reasons of performance and clarity.
All operators will eventually be implemented in assembly.

