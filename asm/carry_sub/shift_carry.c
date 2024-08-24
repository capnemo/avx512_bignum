#include <stdint.h>

void shift_carry(int64_t* num)
{
    int64_t carry = 0;
    int64_t base = 0x100000000;
    for (int i = 7; i >= 0; i--) {
        num[i] += carry;
        carry = 0;
        if (num[i] < 0) {
            carry = -1; 
            num[i] += base;
        }
    }   
}
