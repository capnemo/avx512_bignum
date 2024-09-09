#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

void print_64(uint64_t* arr);
void print_64_2(uint64_t* arr);
void carry(uint64_t *n);
void print_array_32(uint64_t* arr);
void carry_con(uint64_t *n);

int main(int argc, char *argv[])
{
    uint64_t p = 0xFFFFFFFFL;
    uint64_t num[][8] = {{0, p, p, p, p, p, p, p + 1 + 7},
                         {2, p, p, p, p, p, p, p},
                         {0, p, p, p, p + 1 + 7, p, p, p},
                         {1, p, p, p, p, p, p, p + 1 + 7}};
     
    for (int i = 0; i < sizeof(num)/sizeof(num[0]); i++) {
        print_64(num[i]);
        print_64_2(num[i]); //Run this function only in a loop and we get garbage
                            //when built with -Ofast

        uint64_t* avx_array = (uint64_t *)aligned_alloc(64, 64);
        memcpy(avx_array, num[i], sizeof(num[i]));
        carry(avx_array);
        printf("carry     ");
        print_64_2(avx_array);

        printf("carry con ");
        print_64_2(num[i]);
        carry_con(num[i]);
        printf("carry con ");
        print_64_2(num[i]);
        printf("-----------------------------\n");
    }
}

void carry_con(uint64_t *n)
{
    uint32_t* t32_ptr = (uint32_t *)n;
    uint64_t* t64_ptr = n;
    for (int i = 6; i >= 0; i--) {
        t64_ptr[i] += (uint64_t)(t32_ptr[2 * (i + 1) + 1]);
        t32_ptr[2 * (i + 1) + 1] = 0;
    }
}

void print_64_2(uint64_t* arr)
{
    uint32_t* a = (uint32_t *)arr;
    for (int i = 0; i < 16; i++)  
        printf("%"PRIu32",", a[i]);
    printf("\n");
}

void print_64(uint64_t* arr)
{
    for (int i = 0; i < 8; i++)  
        printf("%llu,", arr[i]);

    printf("\n");
}

void print_array_32(uint64_t* arr)
{
    uint32_t *x = (uint32_t *)arr;
    for (int i = 0; i < 16; i++) 
        printf("%"PRIu32",", x[i]);
    printf("\n");
}

