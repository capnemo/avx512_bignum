#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void mul_array(uint64_t* arr, uint64_t num);
void print_u64(uint64_t* n);
int main(int argc, char *argv[])
{
    uint64_t all_bits = 0xFFFFFFFFL;
    uint64_t arr[] = { all_bits, all_bits, all_bits, all_bits, all_bits, 
                       all_bits, all_bits, all_bits};

    uint64_t *al64;
    posix_memalign((void **)&al64, 64, 64);
    memcpy((void *)al64, arr, sizeof(arr));

    uint64_t m = 2L;
    
    print_u64(al64);
    printf("%lu\n", m);
    mul_array(al64, m);
    print_u64(al64);

    uint64_t a[] = {0,1,2,3,4,5,6,7};
    m *= 10L;
    memcpy((void *)al64, a, sizeof(a));

    print_u64(al64);
    printf("%lu\n", m);
    mul_array(al64, m);
    print_u64(al64);
}

void print_u64(uint64_t* n)
{
    for (int i = 0; i < 8; i++)
        printf("%lu ", n[i]);
    printf("\n");
}
