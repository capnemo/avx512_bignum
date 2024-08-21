#include <immintrin.h>
#include <stdint.h>

const uint64_t half_mask = 0xFFFFFFFFL;
const uint64_t full_mask = (half_mask << 32) + half_mask;
__m512i mask = {full_mask, half_mask, half_mask, half_mask, half_mask, half_mask,
                half_mask, half_mask};

const uint64_t zero_loc = 14*(0x100000000 + 1);
const uint64_t zx = 14*0x100000000;
__m512i indexes[] =
{{ zero_loc, zero_loc, zero_loc, zero_loc, zero_loc, zero_loc, zx + 15, zero_loc},
 { zero_loc, zero_loc, zero_loc, zero_loc, zero_loc, zx + 13, zero_loc, zero_loc},
 { zero_loc, zero_loc, zero_loc, zero_loc, zx + 11, zero_loc, zero_loc, zero_loc},
 { zero_loc, zero_loc, zero_loc, zx + 9, zero_loc, zero_loc, zero_loc, zero_loc},
 { zero_loc, zero_loc, zx + 7, zero_loc, zero_loc, zero_loc, zero_loc, zero_loc},
 { zero_loc, zx + 5, zero_loc, zero_loc, zero_loc, zero_loc, zero_loc, zero_loc},
 { zx + 3, zero_loc, zero_loc, zero_loc, zero_loc, zero_loc, zero_loc, zero_loc}};


void carry(__m512i *n)
{
    uint32_t *t32_ptr = (uint32_t *)n;
    uint32_t last_number = t32_ptr[14];
    t32_ptr[14] = 0;

    for (int i = 0; i < 7; i++) {
        __m512i s = _mm512_permutexvar_epi32(indexes[i], *n);
        *n = _mm512_add_epi64(*n, s);
    }
    *n = _mm512_and_si512(*n, mask);

    t32_ptr[14] = last_number;
}


