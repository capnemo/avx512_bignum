#include <iostream>
#include "b10.h"
#include "b32.h"

template <typename T>
void b32_to_b10(uint64_t b32_num, T& b10_num);
void accumulate(vec32& acc, vec32& n);
bool is_zero(const vec32& v);
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

    void print_b10(const vec32& num)
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
        vec32 base_exponent = {1};
        vec32 totals;
        b32_to_b10(num[num.size() - 1], totals);
        for (int i = num.size() - 2; i >= 0; i--) {
            raise_base(base_exponent);
            vec32 current_dw;
            multiply_base(num[i], base_exponent, current_dw);
            accumulate(totals, current_dw);
        }

        n10 = vec8(totals.size(), 0);
        uint64_t carry = 0;
        for (int i = totals.size() - 1; i >= 0; i--) {
            uint64_t tp = (uint64_t)totals[i] + (uint64_t)carry;
            carry = tp / (uint64_t)10;
            n10[i] = tp % (uint64_t)10;
        }

        if (carry != 0) 
            b32_to_b10(carry, n10);
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

/*  Multuplies the positional value with the current power of the base(2^32)
 *  IN: p_val positional value
 *  IN: base_exp current power
 *  OUT: prod, the product.
 */
void multiply_base(uint32_t p_val, const vec32& base_exp, vec32& prod)
{
    
    prod = vec32(base_exp.size(), 0);
    uint64_t carry = 0;
    for (int i = base_exp.size() - 1; i >= 0; i--) {
        uint64_t p = ((uint64_t)base_exp[i] * (uint64_t)p_val) + (uint64_t)carry;
        prod[i] = p % 10;
        carry = p / 10;
    }

    if (carry != 0)
        b32_to_b10(carry, prod);
}

/*  Computes the next power of 2^32
 *  IN: current_power
 *  OUT: current_power, the next power
 */
void raise_base(vec32& current_power)
{
    uint64_t base = 4294967296;
    uint64_t carry = 0;
    for (int i = current_power.size() - 1; i >= 0; i--) {
        uint64_t tp = (uint64_t)current_power[i] * base + carry;
        carry = tp / 10;
        current_power[i] = tp % 10;
    }

    if (carry != 0)
        b32_to_b10(carry, current_power);
}

/*  Computes acc += n.
 *  IN: acc, vector of uint32_t
 *  IN: n, vector of uint32_t
 *  OUT: acc
 */
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
        tp.w = (uint64_t)lg_vec[lg_in] + (uint64_t)sm_vec[sm_in] + (uint64_t)carry;
        carry = tp.a[1];
        lg_vec[lg_in] = tp.a[0];
        lg_in--;
        sm_in--;
    }

    while (lg_in >= 0) {
        split_64 tp;
        tp.w = (uint64_t)lg_vec[lg_in] + (uint64_t)carry;
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
template <typename T>
void b32_to_b10(uint64_t b32_num, T& b10_num)
{
    if (b32_num == 0) {
        b10_num.push_back(0);
        return;
    }
          
    b10_num.reserve(10);
    while (b32_num > 0) {
        b10_num.insert(b10_num.begin(), b32_num % 10);
        b32_num /= 10;
    }
}

/*  Returns if v values to 0.
 *  IN: v vector of uint32_t
 */
bool is_zero(const vec32& v)
{
    if (v.size() == 0)
        return true;
    
    if ((v.size() == 1) && (v[0] == 0)) 
        return true;

    return false;
}
