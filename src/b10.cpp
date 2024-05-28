#include <iostream>
#include "b10.h"
#include "b32.h"

void b10_multiply(vec32& m1, const vec32& m2);
void b32_to_b10(uint64_t b32_num, vec32& b10_num);
void accumulate(vec32& acc, vec32& n);
bool is_zero(const vec32& v);
void normalize_to_b10(vec32& v32);

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
        vec32 base_vec;
        b32_to_b10(base_32, base_vec);
        vec32 base_exponent = {1};
        vec32 totals;
        b32_to_b10(num[num.size() - 1], totals);
        for (int i = num.size() - 2; i >= 0; i--) {
            vec32 current_dw;
            b32_to_b10(num[i], current_dw);
            b10_multiply(base_exponent, base_vec);
            normalize_to_b10(base_exponent);
            b10_multiply(current_dw, base_exponent);
            accumulate(totals, current_dw);
        }

        n10.clear();
        n10.insert(n10.begin(), totals.begin(), totals.end());
    }

    /* Converts a uint32_t vector to a base 10 string using b32::divide_by
     * Immeasurably inefficient. Optimizing divide_by might make it better
     * IN: num
     * OUT: n10
     */
    void convert_to_b10_divide(const b32& num, std::string& n10)
    {
        b32 nmtor = num;
        b32 dntor({10});
        while(nmtor.is_zero() == false) {
            nmtor.divide_by(dntor);
            n10.insert(n10.begin(), nmtor.get_remainder_msb() + '0');
        }
    }

    /* Converts a uint32_t vector to a base 10 string
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

void accumulate(vec32& acc, vec32& n)
{
    if (is_zero(n) == true) 
        return;

    if (is_zero(acc) == true) {
        acc.clear();
        acc.insert(acc.begin(), n.begin(), n.end());
        return;
    }

    if (n.size() > acc.size())  
        acc.insert(acc.begin(), n.size() - acc.size(), 0);

    int ac_in = acc.size() - 1;
    int n_in = n.size() - 1;
    uint64_t carry = 0;
    while (n_in >= 0) {
        uint64_t tp = (uint64_t)acc[ac_in] + (uint64_t)n[n_in] + carry;
        acc[ac_in] = tp % 10;
        carry = tp / 10;
        ac_in--;
        n_in--;
    }
    
    while (ac_in >= 0) {
        if (carry == 0)
            break;
        uint64_t tp = acc[ac_in] + carry;
        acc[ac_in] = tp % 10;
        carry = acc[ac_in] / 10;
        ac_in--;
    }

    if (carry != 0) {
        vec32 dig_vec;
        b32_to_b10(carry, dig_vec);
        acc.insert(acc.begin(), dig_vec.begin(), dig_vec.end());
    }
}

/*  Turns a 32 bit number into a b10 vector
 *  IN: b32_num, 32 bit number
 *  OUT: b10_num 
 */
void b32_to_b10(uint64_t b32_num, vec32& b10_num)
{
    if (b32_num == 0) {
        b10_num.push_back(0);
        return;
    }
          
    b10_num.reserve(9);
    while (b32_num > 0) {
        b10_num.insert(b10_num.begin(), b32_num % 10);
        b32_num /= 10;
    }
}

/* Computes m1 *= m2 in base 10.
 * IN: m1, m2
 * OUT: m1
 */
void b10_multiply(vec32& m1, const vec32& m2)
{
    if (is_zero(m2) == true) {
        m1.clear();
        m1[0] = 0;
        return;
    }

    if (is_zero(m1) == true)
        return;

    int m1_sz = m1.size();
    int m2_sz = m2.size();
    std::vector<uint64_t> prod_list((m1_sz + m2_sz - 1), 0);
    for (int i = 0; i < m1.size(); i++)  {
        if (m1[i] == 0)
            continue;
        for (int j = 0; j < m2.size(); j++) 
            prod_list[i + j] += (uint64_t)m1[i] * (uint64_t)m2[j];
    }

    m1.clear();
    m1.insert(m1.begin(), prod_list.begin(), prod_list.end());
    //normalize_to_b10(m1);
}

void normalize_to_b10(vec32& v32)
{
    uint64_t carry = 0;
    for (int i = v32.size() - 1; i >= 0; i--) {
        uint64_t tp = (uint64_t)v32[i] + carry;
        v32[i] = tp % (uint64_t)10;
        carry = tp / (uint64_t)10;
    }

    if (carry != 0) {
        vec32 dig_vec;
        b32_to_b10(carry, dig_vec);
        v32.insert(v32.begin(), dig_vec.begin(), dig_vec.end());
    }
}

bool is_zero(const vec32& v)
{
    if (v.size() == 0)
        return true;
    
    if ((v.size() == 1) && (v[0] == 0)) 
        return true;

    return false;
}
