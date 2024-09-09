#include <emmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>

void avx512_add(__m512i* a, __m512i* b)
{
    *a = _mm512_add_epi64(*a,*b);
}
