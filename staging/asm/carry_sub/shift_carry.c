#include <stdint.h>

/*
const uuint64_t half_mask = 0xFFFFFFFF;
const uuint64_t mask = (half_mask << 32) + half_mask;
*/
const uint64_t mask = 1L << 63;

void shift_carry(uint64_t* num)
{
    uint64_t carry = 0;
    uint64_t base = 0x100000000;
    for (int i = 7; i >= 0; i--) {
        num[i] += carry;
        carry = 0;
        if ((num[i] & mask) != 0) {
            carry = -1; 
            num[i] += base;
        }
    }   
}
