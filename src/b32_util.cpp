#include "b32.h"
#include "b10.h"
#include <string.h>
#include <bitset>

bool b32::is_zero() const
{
    if (num.size() == 0)
        return true;

    if ((num.size() == 1) && (num[0] == 0))
        return true;

    return false;
}

void b32::shift_left_digits(uint32_t len)
{
    num.insert(num.end(), len, 0);
}

void b32::print_vec() const
{
    for (auto m:num)
        std::cout << m << " ";
    std::cout << std::endl;
}

void b32::get_base10_num(std::string& str)
{
    vec8 b10_arr;
    b10::convert_to_b10(num, b10_arr);
    for (auto m:b10_arr) 
        str += (m + '0');
}

uint32_t b32::get_msb() const
{
    if (num.size() == 0)
        return 0;

    return num[0];
}

int b32::compare_msb(const b32& cmp) const
{
    return get_msb() - cmp.get_msb();
}

void b32::flip_sign()
{
    is_negative = !is_negative;
}

size_t b32::get_array_size() const
{
    return num.size();
}

size_t b32::get_array_msb_index() const
{
    return ((get_array_size() - 1) * 32) + get_msb_index(num[0]);
}

//To be used only for num[0] of a b32 object.
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

void b32::set_zero()
{
    num.clear();
    num.push_back(0);
}

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

bool b32::is_unity() const
{
    if ((num.size() == 1) && (num[0] == 1))
        return true;

    return false;
}

void b32::shift_left_array(uint32_t width)
{
    shift_left(num, width%32);

    uint32_t element_shift = width/32;
    if (element_shift != 0)
        num.insert(num.end(), element_shift, 0);
}

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

void b32::shift_right_array(uint32_t width)
{
    if (width == 0)
        return;

    uint32_t element_shift = width/32;
    if (element_shift != 0)
        num.erase(num.end() -  element_shift, num.end());

    shift_right(num, width%32);
}

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

void b32::print_bits() const
{
    for (auto m:num) {
        std::bitset<32> b = m;
        std::cout << b.to_string() << '|';
    }
    std::cout << std::endl;
}

void b32::trim_leading_zeros()
{
    int i = 0;
    while(num[i] == 0) 
        i++;

    if (i != 0)
        num.erase(num.begin(), num.begin() + i);
}
