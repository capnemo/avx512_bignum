#include <iostream>
#include "b10.h"

void add_to(vec8& acc, const vec8& n); 
void multiply_with_2(vec8& cp);

namespace b10 {
void print_b10(const vec8& num)
{
    for (auto n:num)
        printf("%d", n);
    printf("\n");
}

void convert_to_b10(const vec32& num, vec8& n10) 
{
    vec8 cp2 = {1};
    for (int i = num.size() - 1; i >= 0; i--) {
        uint32_t current_byte = num[i];
        uint32_t mask = 1;
        for (int i = 0; i < 32; i++) {
            if ((mask & current_byte) == mask)  
                add_to(n10, cp2);
            mask = mask << 1;
            multiply_with_2(cp2);
        }
    }
}

void convert_to_b10(const b32& num, std::string& n10)
{
    vec8 digs_b10;
    vec32 b32_vec;
    num.get_vector(b32_vec);
    convert_to_b10(b32_vec, digs_b10);

    if (digs_b10.size() == 0)
        digs_b10.push_back(0);

    n10.clear();
    if (num.is_less_than_zero() == true)
        n10 += '-';

    for (auto n:digs_b10) 
        n10 += (n + '0');
}

} //namespace

void add_to(vec8& acc, const vec8& n) 
{
    if (acc.size() < n.size()) 
        acc.insert(acc.begin(), n.size() - acc.size(), 0);

    int an = acc.size() - 1;
    int nn = n.size() - 1;
    uint8_t carry = 0;
    while (nn >= 0) {
        uint8_t tp = acc[an] + n[nn] + carry;
        acc[an] = tp % 10;
        carry = tp / 10;
        an--;
        nn--;
    }

    if (carry == 0)
        return;

    while (an >= 0) {
        uint8_t tp = acc[an] + carry;
        acc[an--] = tp % 10;
        carry = tp / 10;
        if (carry == 0)
            break;
    }

    if (carry != 0)  
        acc.insert(acc.begin(), 1, carry);
}

void multiply_with_2(vec8& cp)
{
    uint8_t carry = 0;
    for (int i = cp.size() - 1; i >= 0; i--) {
        uint8_t tp = (cp[i] << 1) + carry;
        cp[i] = tp % 10;
        carry = tp / 10;
    }

    if (carry != 0)
        cp.insert(cp.begin(), carry);
}
