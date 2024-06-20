#include "b32.h"
#include "b10.h"
#include <string.h>
#include <bitset>

/*
 * Returns if the the object is zero.
 * OUT: true if the object is zero.
 */
bool b32::is_zero() const
{
    if (num.size() == 0)
        return true;

    if ((num.size() == 1) && (num[0] == 0))
        return true;

    return false;
}

/*
 * Prints the 32 bit vector (num)
 */
void b32::print_vec() const
{
    for (auto m:num)
        std::cout << m << " ";
    std::cout << std::endl;
}

/*
 * Converts the b32 object to vec8 (base 10)
 */
void b32::get_base10_num(std::string& str)
{
    vec8 b10_arr;
    b10::convert_to_b10(num, b10_arr);
    for (auto m:b10_arr) 
        str += (m + '0');
}

/*
 * Returns the most significant part of the b32 object.
 */
uint32_t b32::get_msb() const
{
    if (num.size() == 0)
        return 0;

    return num[0];
}

/*
 * Returns the difference between the msb of *this and cmp
 * IN cmp
 * OUT msb difference
 */
int b32::compare_msb(const b32& cmp) const
{
    return get_msb() - cmp.get_msb();
}

/*
 * Flips the sign of the object (+ -> - | - -> +)
 */
void b32::flip_sign()
{
    is_negative = !is_negative;
}

/*
 * Returns the size of the uint32_t array
 */
size_t b32::get_array_size() const
{
    return num.size();
}

/*
 * Returns the position of the most significant bit in the 32 bit array
 */
size_t b32::get_array_msb_index() const
{
    return ((get_array_size() - 1) * 32) + get_msb_index(num[0]);
}

/*
 * Returns the position of the most significant bit of element 0 of the 
 * 32 bit array
 */
size_t b32::get_msb_index(uint32_t n) const 
{
    uint32_t mask = 0x80000000;
    size_t index = 32;

    for (int i = 0; i <= 31; i++) {
        if ((n & mask) != 0)
            return index;
        mask = mask >> 1;
        index--;
    }

    return 0;
}

/*
 * Set the object value to zero.
 */
void b32::set_zero()
{
    num.clear();
    num.push_back(0);
    is_negative = false;
}

/*
 *  Returns:
 *  > 0 if *this > cmp
 *  0 if *this == cmp
 *  < 0 if *this < cmp
 *  IN: cmp, another b32 object
 */
int b32::compare_abs(const b32& cmp) const
{
    size_t cm_sz = cmp.get_array_size();
    size_t nm_sz = get_array_size();
    
    if (cm_sz != nm_sz)
        return (nm_sz - cm_sz);

    vec32 cm_vec;
    cmp.get_vector(cm_vec);
    for (int i = 0; i < nm_sz; i++) {
        if (cm_vec[i] != num[i])  
            return (num[i] > cm_vec[i]) ? 1:-1;
    }
    return 0;
}

/*
 *  Returns if the object value is 1.
 */
bool b32::is_unity() const
{
    if ((num.size() == 1) && (num[0] == 1))
        return true;

    return false;
}

/*
 *  Shifts the entire uint32_t array left by width bits.
 *  IN: width, shift width
 */
void b32::shift_left_array(uint32_t width)
{
    if (width == 0)
        return;

    shift_left(num, width%32);

    uint32_t element_shift = width/32;
    if (element_shift != 0)
        num.insert(num.end(), element_shift, 0);
}

/*
 *  Shifts the entire uint32_t array left by width bits.
 *  IN: width, shift width. width <= 32
 */
void b32::shift_left(vec32& digits, uint8_t width)
{
    if (width == 0)
        return;

    uint32_t mask = 0xFFFFFFFF << (32 - width);
    digits.insert(digits.begin(), 0);
    for (int i = 1; i < digits.size(); i++) {
        uint32_t leading_bits = digits[i] & mask;
        uint32_t trailing_bits = leading_bits >> (32 - width);
        digits[i - 1] = digits[i - 1] | trailing_bits;
        digits[i] = digits[i] << width;
    }

    if (digits[0] == 0)
        digits.erase(digits.begin());
}

/*
 *  Shifts the entire uint32_t array right by width bits.
 *  IN: width, shift width
 */
void b32::shift_right_array(uint32_t width)
{
    if (width == 0)
        return;

    uint32_t element_shift = width/32;
    if (element_shift != 0)
        num.erase(num.end() -  element_shift, num.end());

    shift_right(num, width%32);
}

/*
 *  Shifts the entire uint32_t array right by width bits.
 *  IN: width, shift width. width <= 32
 */
void b32::shift_right(vec32& digits, uint8_t width)
{
    uint32_t mask = (0xFFFFFFFF << (32 - width)) >> (32 - width);
    
    for (int i = digits.size() - 1; i > 0; i--) {
        uint32_t trailing_mask = digits[i - 1] & mask;
        uint32_t leading_mask = trailing_mask << (32 - width);
        digits[i] >>= width;
        digits[i] |= leading_mask;
    }

    digits[0] >>= width;
}

/*
 * Prints the uint32_t array as a base 2 number.
 */
void b32::print_bits() const
{
    for (auto m:num) {
        std::bitset<32> b = m;
        std::cout << b.to_string() << '|';
    }
    std::cout << std::endl;
}

/*
 * Removes leading zeros from the uint32_t array.
 */
void b32::trim_leading_zeros()
{
    int i = 0;
    while(num[i] == 0) 
        i++;

    if (i != 0)
        num.erase(num.begin(), num.begin() + i);
}

/* 
 * Converts b10_num to a uint32_t array and instantiates *this with it.
 * IN b10_num
 */
void b32::convert_to_b32(const std::string& b10_num)
{
    bool is_neg = false;
    std::string b10_str = b10_num;
    if (b10_str[0] == '-') {
        b10_str.erase(b10_str.begin());
        is_neg = true;
    }

    vec8 b10_vec;
    for (auto m:b10_str)
        b10_vec.push_back(m - '0');

    convert_to_b32(b10_vec, is_neg);
}

/* 
 * Converts a b10 vector into a vector of 10^9 numbers. 
 * IN: b10_num 
 * OUT: b10p9_num 
 */
void b32::convert_to_b10p9(const vec8& b10_num, vec32& b10p9_num)
{
    std::string num_str;
    for (int i = b10_num.size() - 1; i >= 0; i--) {
        num_str.insert(num_str.begin(), b10_num[i] + '0');
        if (num_str.size() == 9) {
            b10p9_num.insert(b10p9_num.begin(), 
                             strtoul(num_str.c_str(), nullptr, 10));
            num_str = "";
        }
    }

    b10p9_num.insert(b10p9_num.begin(), 
                     strtoul(num_str.c_str(), nullptr, 10));
}

/* 
 * Converts uint8_t array to a uint32_t array and instantiates *this with it.
 * IN: b10_num: uint8_t array
 * IN: under_zero: Sign of the number
 */
void b32::convert_to_b32(const vec8& b10_num, bool under_zero)
{
    if (b10_num.size() == 0) {
        num = vec32({0});
        return;
    }
    
    uint32_t b10p9 = 1'000'000'000;
    b32 power_10({1});

    vec32 b10p9_digs;
    convert_to_b10p9(b10_num, b10p9_digs);

    int sz = b10p9_digs.size();
    num = vec32(1, b10p9_digs[sz - 1]);
    for (int i = sz - 2; i >= 0; i--) {
        power_10.multiply_with_b10_digit(b10p9);
        b32 dig_power = power_10;
        dig_power.multiply_with_b10_digit(b10p9_digs[i]);
        add_to(dig_power);
    }

    is_negative = under_zero;

}

/*
 * Resolves sign prior to an operation. Use for multiplication and division
 * only.
 * IN: cmp: another b32 object
 */
void b32::resolve_signs(const b32& cmp)
{
    if (is_less_than_zero() == cmp.is_less_than_zero()) {
        if (is_negative == true)
            is_negative = false;
    } else 
        is_negative = true;
}

