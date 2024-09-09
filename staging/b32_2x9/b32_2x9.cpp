#include <iostream>
#include <iomanip>
#include <cstring>
#include "b32_2x9.h"

void b32_2x9::set_number(uint64_t* n, uint32_t sz)
{
    if (num[0] != nullptr)
        allctr.deallocate(num);
        
    uint32_t pref = register_length - (sz % register_length);
    allctr.allocate(num, sz, pref);
    memcpy(num[0] + pref, n, sz * sizeof(uint64_t));
    leading_zeros = pref;
}

void b32_2x9::shift_right(uint32_t shift_len)
{
    reset_number(-1 * shift_len);
}

void b32_2x9::shift_left(uint32_t shift_len)
{
    reset_number(shift_len);
}

void b32_2x9::reset_number(int32_t shift_len)
{
    if (shift_len == 0)
        return;
    
    uint32_t osz = num.size() * register_length - leading_zeros;
    uint32_t cp_sz = osz;
    if (shift_len < 0) 
        cp_sz += shift_len;

    uint32_t new_sz = osz + shift_len;
    au64_vec new_num;
    uint32_t pad;
    allctr.allocate(new_num, new_sz, pad);
    memcpy(new_num[0] + pad, num[0] + leading_zeros, 
           cp_sz * sizeof(uint64_t));

    allctr.deallocate(num);
    num = std::move(new_num);
    leading_zeros = pad;
}

uint32_t b32_2x9::get_actual_size()
{
    return num.size() * register_length - leading_zeros;
}

void b32_2x9::get_copy(uint64_t** buff)
{
    memcpy(*buff, num[0] + leading_zeros, 
            get_actual_size() * sizeof(uint64_t));
}

void b32_2x9::find_leading_zeros()
{
    uint64_t* ptr = num[0];
    leading_zeros = 0;
    while (ptr[leading_zeros] == 0) 
        leading_zeros++;
}

void b32_2x9::print_vec()
{
    for (int i = 0; i < num.size(); i++) {
        uint64_t *p = num[i];
        for (int j = 0; j < register_length; j++) 
            std::cout << std::right << std::setw(10) << p[j] << " ";
        std::cout << std::endl;
    }
}

b32_2x9::~b32_2x9()
{
    allctr.deallocate(num);
    allctr.release_all();
}
