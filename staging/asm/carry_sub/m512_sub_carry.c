#include <immintrin.h>
#include <stdint.h>
const uint64_t qw_carry = (0x100000000 + 1);
__m512i msb_in[] =
{{ 0, 0, 0, 0, 0, 0, 15 * qw_carry, 0},
 { 0, 0, 0, 0, 0, 13 * qw_carry, 0, 0},
 { 0, 0, 0, 0, 11 * qw_carry, 0, 0, 0},
 { 0, 0, 0, 9 * qw_carry, 0, 0, 0, 0},
 { 0, 0, 7 * qw_carry, 0, 0, 0, 0, 0},
 { 0, 5 * qw_carry, 0, 0, 0, 0, 0, 0},
 { 3 * qw_carry, 0, 0, 0, 0, 0, 0, 0}};

__m512i shift_in[] =

{{ 0, 0, 0, 0, 0, 0, 0, 15},
{ 0, 0, 0, 0, 0, 0, 13, 0},
{ 0, 0, 0, 0, 0, 11, 0, 0},
{ 0, 0, 0, 0, 9, 0, 0, 0},
{ 0, 0, 0, 7, 0, 0, 0, 0},
{ 0, 0, 5, 0, 0, 0, 0, 0},
{ 0, 3, 0, 0, 0, 0, 0, 0}};

void carry_sub(__m512i *n)
{
    uint32_t *t32_ptr = (uint32_t *)n;
    uint32_t first_number = t32_ptr[0];
    t32_ptr[0] = 0;

    for (int i = 0; i < 7; i++) {
        __m512i s = _mm512_permutexvar_epi32(msb_in[i], *n);
        __m512i t = _mm512_permutexvar_epi32(shift_in[i], *n);

        t = _mm512_srli_epi64(t, 31);
        t = _mm512_slli_epi64(t, 32);

        *n = _mm512_add_epi64(*n, t);
        *n = _mm512_add_epi64(*n, s);
    }
    (*n)[0] += first_number;
}

