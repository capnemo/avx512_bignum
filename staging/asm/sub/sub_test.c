#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Switch to uuint64_t or we switch everything to uint64_t
void shift_carry(uint64_t* num);
void avx512_sub(uint64_t* a, uint64_t* b);
void print_num(uint64_t* num);
void run_sub(uint64_t* a1, uint64_t* a2);

uint32_t all_bits = 0xFFFFFFFFL;

int main(int argc, char *argv[])
{
    uint64_t a[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    uint64_t b[8] = {0, 0, 0, 0, 0, 0, 0, 1};
    run_sub(a, b);

    uint64_t c[8] = {7, 6, 5, 4, 3, 2, 1, 0};
    uint64_t d[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    run_sub(c, d);
}

void run_sub(uint64_t* a1, uint64_t* a2)
{
    uint64_t *x,*y;
    posix_memalign((void **)&x, 64, 64);
    posix_memalign((void **)&y, 64, 64);
    memcpy(x, a1, 64);
    memcpy(y, a2, 64);
    
    avx512_sub(x, y);

    print_num(x);
    shift_carry(x);
    print_num(x);
    free(x);
    free(y);
}

void print_num(uint64_t* num)
{
    for (int i = 0; i < 8; i++)
        printf("%ld ", num[i]);
    printf("\n");
}

