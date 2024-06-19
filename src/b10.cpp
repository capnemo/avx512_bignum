#include <iostream>
#include "b10.h"
#include "b32.h"

void b10p9_to_b10(uint64_t b32_num, vec8& b10_num);
void accumulate(vec32& acc, vec32& n);
bool is_zero(const vec32& v);
void raise_base(vec32& current_power);
void multiply_base(uint32_t p_val, const vec32& base_exp, vec32& prod);
static const uint64_t b10p9 = 1'000'000'000;
template <typename T>
void print_b10(const T& num, std::string pref = "");

namespace b10 {
    
    /* Converts a uint32_t vector to a uint8_t vector
     * First, conversion to base 10^9. All operations are 
     * done in base 10^9. Final step is conversion to base 10.
     * IN: num
     * OUT: n10
     */
    void convert_to_b10(const vec32& num, vec8& n10) 
    {
        vec32 base_exponent = {1};
        vec32 totals({(uint32_t)(num[num.size() - 1] % b10p9)});
        uint32_t div = (uint32_t)(num[num.size() - 1] / b10p9);
        if (div != 0) 
            totals.insert(totals.begin(), div);

        for (int i = num.size() - 2; i >= 0; i--) {
            raise_base(base_exponent);
            vec32 current_dw;
            multiply_base(num[i], base_exponent, current_dw);
            accumulate(totals, current_dw);
        }
        
        b10p9_to_b10(totals[0], n10);
        for (int i = 1; i < totals.size(); i++)  {
            vec8 digs10;
            digs10.reserve(9);
            b10p9_to_b10(totals[i], digs10);
            digs10.insert(digs10.begin(), 9 - digs10.size(), 0);
            n10.insert(n10.end(), digs10.begin(), digs10.end());
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

void b10p9_to_b10(uint64_t b32_num, vec8& b10_num)
{
    while (b32_num > 0) {
        b10_num.insert(b10_num.begin(), b32_num % 10);
        b32_num /= 10;
    }
}

/*  Multuplies the positional value with the current power of the base(2^32)
 *  IN: p_val positional value
 *  IN: base_exp current power
 *  OUT: prod, the product.
 */
void multiply_base(uint32_t p_val, const vec32& base_exp, vec32& prod)
{
    
    prod = vec32(base_exp.size(), 0);
    uint32_t carry = 0;
    for (int i = base_exp.size() - 1; i >= 0; i--) {
        uint64_t p = ((uint64_t)base_exp[i] * (uint64_t)p_val) + 
                     (uint64_t)carry;
        prod[i] = p % b10p9;
        carry = p / b10p9;
    }

    if (carry != 0)
        prod.insert(prod.begin(), carry);
}

/*  Computes the next power of 2^32
 *  IN: current_power
 *  OUT: current_power, the next power
 */
void raise_base(vec32& current_power)
{
    vec32 base_32 = {4,294967296};
    uint64_t carry = 0;
    for (int i = current_power.size() - 1; i >= 0; i--) {
        vec64 tp(2,0);
        tp[0] = (uint64_t)current_power[i] * (uint64_t)base_32[0];
        tp[1] = (uint64_t)current_power[i] * (uint64_t)base_32[1] + carry;
        current_power[i] = tp[1] % b10p9;
        tp[0] += tp[1] / b10p9;
        carry = tp[0];
    }

    if (carry != 0) {
        uint32_t new_carry = (uint64_t)carry / (uint64_t)b10p9;
        carry = carry % b10p9;
        current_power.insert(current_power.begin(), carry);
        if (new_carry != 0)
            current_power.insert(current_power.begin(), new_carry);
    }
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
    bool copy_required = false;
    if (acc.size() >= n.size()) {
        lg_ptr = &acc;
        sm_ptr = &n;
    } else {
        lg_ptr = &n;
        sm_ptr = &acc;
        copy_required = true;
    }

    vec32& lg_vec = *lg_ptr;
    vec32& sm_vec = *sm_ptr;
    std::vector<uint32_t> sum_vec;
    int lg_in = lg_vec.size() - 1;
    int sm_in = sm_vec.size() - 1;
    uint32_t carry = 0;
    while (sm_in >= 0) {
        uint64_t sum = (uint64_t)lg_vec[lg_in] + (uint64_t)sm_vec[sm_in] + 
                       (uint64_t)carry;
        lg_vec[lg_in] = sum % b10p9;
        carry = sum / b10p9;

        lg_in--;
        sm_in--;
    }

    while (lg_in >= 0) {
        uint64_t sum = (uint64_t)lg_vec[lg_in] + (uint64_t)carry;
        carry = sum / b10p9;
        lg_vec[lg_in--] = sum % b10p9;
    }

    if (carry != 0)
        lg_vec.insert(lg_vec.begin(), carry);

    if (copy_required == true)
        acc = std::move(lg_vec); 
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
    
/* Prints the contents of num to stdout.
* IN: num
* IN: pref, printed before the vector.
*/
template <typename T>
void print_b10(const T& num, std::string pref)
{
    std::cout << pref << ((pref == "") ? "":" ");

    for (auto n:num)
        std::cout << (uint32_t)n << " ";
    std::cout << std::endl;
}
