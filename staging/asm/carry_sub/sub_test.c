#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Switch to uint64_t or we switch everything to int64_t
void shift_carry(int64_t* num);
void avx512_sub(int64_t* a, int64_t* b);
void print_num(int64_t* num);

uint32_t all_bits = 0xFFFFFFFFL;
int main(int argc, char *argv[])
{
    int64_t a[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    int64_t b[8] = {0, 0, 0, 0, 0, 0, 0, 1};

    int64_t *x,*y,*z,*w;
    posix_memalign((void **)&x, 64, 64);
    posix_memalign((void **)&y, 64, 64);
    memcpy(x, a, sizeof(a));
    memcpy(y, b, sizeof(b));

    avx512_sub(x, y);

    print_num(x);
    shift_carry(x);
    print_num(x);

}

void print_num(int64_t* num)
{
    for (int i = 0; i < 8; i++)
        printf("%ld ", num[i]);
    printf("\n");
}

