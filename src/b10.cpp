#include <iostream>
#include "b10.h"

void add_to(vec8& acc, const vec8& n); 
void b10_multiply(vec8& m1, const vec8& m2);
void b32_to_b10(uint64_t b32_num, vec8& b10_num);

namespace b10 {
    
    /* Prints the contents of num to stdout.
     * IN: num
     */
    void print_b10(const vec8& num)
    {
        for (auto n:num)
            printf("%d", n);
        printf("\n");
    }

    /* Converts a uint32_t vector to a uint8_t vector
     * IN: num
     * OUT: n10
     */
    void convert_to_b10(const vec32& num, vec8& n10) 
    {
        uint64_t base_32 = (uint64_t)(0x100000000);
        vec8 base_vec;
        b32_to_b10(base_32, base_vec);
        vec8 base_exponent = {1};
        b32_to_b10(num[num.size() - 1], n10);
        for (int i = num.size() - 2; i >= 0; i--) {
            vec8 current_dw;
            b32_to_b10(num[i], current_dw);
            b10_multiply(base_exponent, base_vec);
            b10_multiply(current_dw, base_exponent);
            add_to(n10, current_dw);
        }
    }

    /* Converts a uint32_t vector to a string
     * IN: num
     * OUT: n10
      */
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

/*  Turns a 32 bit number into a b10 vector
 *  IN: b32_num, 32 bit number
 *  OUT: b10_num 
 */
void b32_to_b10(uint64_t b32_num, vec8& b10_num)
{
    b10_num.reserve(9);
    while (b32_num > 0) {
        b10_num.insert(b10_num.begin(), b32_num % 10);
        b32_num /= 10;
    }
}

/* Adds two uint8_t vectors. The result is in acc
 * IN: acc, n
 * OUT: acc
 */
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

/* Computes m1 *= m2 in base 10.
 * IN: m1, m2
 * OUT: m1
 */
void b10_multiply(vec8& m1, const vec8& m2)
{
    if (m2.size() == 0)
        return;

    int m1_sz = m1.size();
    int m2_sz = m2.size();
    std::vector<uint32_t> prod_list((m1_sz + m2_sz - 1), 0);
    for (int i = 0; i < m1.size(); i++) {
        if (m1[i] == 0)
            continue;       
    for (int j = 0; j < m2.size(); j++) {
        if (m2[j] == 0)
            continue;
        prod_list[i + j] += m1[i] * m2[j];
        }
    }

    uint32_t carry = 0;
    for (int i = prod_list.size() - 1; i >= 0; i--) {
        uint32_t tp = prod_list[i] + carry;
        prod_list[i] = tp % 10;
       carry = tp / 10;
    }

    if (carry != 0)
        prod_list.insert(prod_list.begin(), carry);

    m1.clear();
    m1.reserve(prod_list.size());
    for (auto m:prod_list) 
        m1.push_back(m);
}
