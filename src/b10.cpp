#include <iostream>
#include "b10.h"

void add_to(vec8& acc, const vec8& n); 
void b10_multiply(vec32& m1, const vec32& m2);
void b32_to_b10(uint64_t b32_num, vec32& b10_num);
void accumulate(vec32& acc, vec32& n);
bool is_zero(const vec32& v);
void print_vec_8(const vec8& v);
void print_vec_32(const vec32& v);
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
            b10_multiply(current_dw, base_exponent);
            accumulate(totals, current_dw);
        }

        n10.clear();
        n10.insert(n10.begin(), totals.begin(), totals.end());
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

void accumulate(vec32& acc, vec32& n)
{
    if (is_zero(n) == true) 
        return;

    if (is_zero(acc) == true) {
        acc.clear();
        acc.insert(acc.begin(), n.begin(), n.end());
        return;
    }

    int sz = std::min(n.size(), acc.size()) - 1;
    if (n.size() > acc.size())  {
        bool is_acc_zero = (acc.size() == 0) ? true:false;
        acc.insert(acc.begin(), n.begin(), 
                   n.begin() + n.size() - acc.size());
        if (is_acc_zero == true)
            return;
    }

    int ac_in = acc.size() - 1;
    int n_in = n.size() - 1;
    while (sz >= 0) {
        acc[ac_in--] += n[n_in--];
        sz--;
    }
    normalize_to_b10(acc);
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
    for (int i = 0; i < m1.size(); i++) 
        for (int j = 0; j < m2.size(); j++) 
            prod_list[i + j] += (uint64_t)m1[i] * (uint64_t)m2[j];

    m1.clear();
    m1.insert(m1.begin(), prod_list.begin(), prod_list.end());
    normalize_to_b10(m1);
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

bool is_zero(const vec8& v)
{
    if (v.size() == 0)
        return true;
    
    if ((v.size() == 1) && (v[0] == 0)) 
        return true;

    return false;
}

bool is_zero(const vec32& v)
{
    if (v.size() == 0)
        return true;
    
    if ((v.size() == 1) && (v[0] == 0)) 
        return true;

    return false;
}

void print_vec_32(const vec32& v) 
{
    for (auto m:v)
        std::cout << m << " ";

    std::cout << std::endl;
}

void print_vec_8(const vec8& v) 
{
    for (auto m:v)
        std::cout << (int)m << " ";

    std::cout << std::endl;
}
