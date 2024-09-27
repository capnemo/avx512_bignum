#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void carry_sub(uint64_t *n);
void print_num(uint64_t* num);
void run_test(uint64_t* arr);

int main(int argc, char *argv[])
{
    uint64_t* x;
    posix_memalign((void **)&x, 64, 64);

    uint64_t a[] = {1,0,0,0,0,0,0,-1};
    memcpy((void *)x, a, 64);
    run_test(x);

    uint64_t b[] = {1,2,3,4,5,6,7,10000};
    memcpy((void *)x, b, 64);
    run_test(x);

    uint64_t c[] = {6,5,4,3,2,1,0,-1};
    memcpy((void *)x, c, 64);
    run_test(x);

    uint64_t d[] = {6,5,4,3,-1,12,67,151};
    memcpy((void *)x, d, 64);
    run_test(x);
}

void run_test(uint64_t* arr)
{
    print_num(arr);
    carry_sub(arr);
    print_num(arr);
}

void print_num(uint64_t* num)
{
    for (int i = 0; i < 8; i++)
        printf("%ld ", num[i]);
    printf("\n");
}

