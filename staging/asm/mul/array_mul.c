#include <immintrin.h>
#include <stdint.h>

void mul_array(__m512i* arr, uint64_t num)
{
    __m512i m2 = _mm512_set1_epi64(num);
    *arr = _mm512_mul_epu32(*arr, m2);
}

