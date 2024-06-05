#include <iostream>
#include "b10.h"
#include "b32.h"

void b10_multiply(vec32& m1, const vec32& m2);
void b32_to_b10(uint64_t b32_num, vec32& b10_num);
void b32_to_string(uint32_t b32_num, std::string& b10_str);
void accumulate(vec32& acc, vec32& n);
bool is_zero(const vec32& v);
void normalize_to_b10(vec32& v32);
void raise_base(vec32& current_power);
void multiply_base(uint32_t p_val, const vec32& base_exp, vec32& prod);

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

    void print_b10(const vec32 num)
    {
        for (auto n:num)
            printf("%u ", n);
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
            raise_base(base_exponent);
            b10_multiply(current_dw, base_exponent);
            accumulate(totals, current_dw);
        }
#if 0
        for (int i = num.size() - 2; i >= 0; i--) {
            raise_base(base_exponent);
            vec32 current_dw;
            multiply_base(num[i], base_exponent, current_dw);
            accumulate(totals, current_dw);
        }
#endif

        n10 = vec8(totals.size(), 0);
        uint64_t carry = 0;
        for (int i = totals.size() - 1; i >= 0; i--) {
            uint64_t tp = totals[i] + carry;
            carry = tp / (uint64_t)10;
            n10[i] = tp % (uint64_t)10;
        }

        if (carry != 0) {
            vec32 dig_vec;
            b32_to_b10(carry, dig_vec);
            n10.insert(n10.begin(), dig_vec.begin(), dig_vec.end());
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

void multiply_base(uint32_t p_val, const vec32& base_exp, vec32& prod)
{
    
    std::cout << "==========================" << std::endl;
    b10::print_b10(base_exp);
    prod = vec32(base_exp.size(), 0);
    uint32_t carry = 0;
    for (int i = base_exp.size() - 1; i >= 0; i--) {
        split_64 tp;
        tp.w = ((uint64_t)base_exp[i] * (uint64_t)p_val) + (uint64_t)carry;
        std::string s = ":";
        std::cout << tp.w << s << base_exp[i] << s << p_val << s << carry << std::endl;
        std::cout << tp.a[0] << s << tp.a[1] << std::endl;
        std::cout << "---------------------------" << std::endl;
        prod[i] = tp.a[0];
        carry = tp.a[1];
    }

    std::cout << "==========================" << std::endl;
    if (carry != 0) 
        prod.insert(prod.begin(), carry);
    b10::print_b10(prod);
}

void raise_base(vec32& current_power)
{
    uint64_t base = 4294967296;
    uint64_t carry = 0;
    for (int i = current_power.size() - 1; i >= 0; i--) {
        uint64_t tp = (uint64_t)current_power[i] * base + carry;
        carry = tp / (uint64_t)10;
        current_power[i] = tp % (uint64_t)10;
    }

    if (carry != 0) {
        vec32 dig_vec;
        b32_to_b10(carry, dig_vec);
        current_power.insert(current_power.begin(), dig_vec.begin(), dig_vec.end());
    }
}

void accumulate(vec32& acc, vec32& n)
{
    if (is_zero(n) == true)
        return;

    if (is_zero(acc) == true) {
        acc = n;
        return;
    }

    vec32* lg_ptr;
    vec32* sm_ptr;
    if (acc.size() >= n.size()) {
        lg_ptr = &acc;
        sm_ptr = &n;
    } else {
        lg_ptr = &n;
        sm_ptr = &acc;
    }

    vec32& lg_vec = *lg_ptr;
    vec32& sm_vec = *sm_ptr;
    std::vector<uint32_t> sum_vec;
    int lg_in = lg_vec.size() - 1;
    int sm_in = sm_vec.size() - 1;
    uint32_t carry = 0;
    while (sm_in >= 0) {
        split_64 tp;
        tp.w = lg_vec[lg_in] + sm_vec[sm_in] + carry;
        carry = tp.a[1];
        lg_vec[lg_in] = tp.a[0];
        lg_in--;
        sm_in--;
    }

    while (lg_in >= 0) {
        split_64 tp;
        tp.w = lg_vec[lg_in] + carry;
        carry = tp.a[1];
        lg_vec[lg_in--] = tp.a[0];
    }

    if (carry != 0)
        lg_vec.insert(lg_vec.begin(), carry);

    acc = lg_vec; //Do it only in case n.size > acc.size. Rethink.
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

void b32_to_string(uint32_t b32_num, std::string& b10_str)
{
    if (b32_num == 0) {
        b10_str = "0";
        return;
    }
          
    b10_str = "";
    while (b32_num > 0) {
        b10_str.insert(0, 1, b32_num % 10 + '0');
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

    uint32_t carry = 0;
    m1 = vec32(prod_list.size(), 0);
    for (int i = prod_list.size() - 1; i >= 0; i--) {
        split_64 tp;
        tp.w = prod_list[i] + carry;
        m1[i] = tp.a[0];
        carry = tp.a[1];
    }

    if (carry != 0)
        m1.insert(m1.begin(), carry);

    //m1.clear();
    //m1.insert(m1.begin(), prod_list.begin(), prod_list.end());
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
