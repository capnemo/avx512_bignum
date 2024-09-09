#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void avx512_add(uint64_t* a, uint64_t* b);

uint32_t all_bits = 0xFFFFFFFFL;
int main(int argc, char *argv[])
{
    uint64_t *x,*y,*z,*w;
    posix_memalign((void **)&x, 64, 64);
    posix_memalign((void **)&y, 64, 64);
    posix_memalign((void **)&z, 64, 64);
    posix_memalign((void **)&w, 64, 64);

    for (int i = 0; i < 8; i++) {
        x[i] = i;
        y[i] = i * 2;
        z[i] = all_bits - i;
        w[i] = i + 5;
    }

    avx512_add(x, y);
    avx512_add(z, w);

    for (int i = 0; i < 8; i++)
        printf("%lu ", x[i]);
    printf("\n");

    for (int i = 0; i < 8; i++)
        printf("%lu ", z[i]);
    printf("\n");
}


