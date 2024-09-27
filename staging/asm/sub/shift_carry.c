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

void new_carry(uint64_t* num)
{
    uint64_t mask = 0x100000000L;
    int32_t carry = 0L;
    for (int i = 7; i >= 0; i--) {
        num[i] += carry;
        uint32_t* parts = (uint32_t *)(num + i);
        carry = parts[1];
        uint64_t tmp = (uint64_t)carry << 1;
        tmp = tmp & mask;
        //num[i] = (int32_t)parts[0] + tmp;
	num[i] += tmp;
    }   
}
