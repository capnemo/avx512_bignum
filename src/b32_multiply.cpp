#include "b32.h"
#include "b10.h"


void b32::multiply_with_10()
{
    b32 base_num = *this;
    b32 sum({0});
    base_num.shift_left_array(1);
    sum.add_to(base_num);
    base_num.shift_left_array(2);
    sum.add_to(base_num);
    
    sum.get_vector(num);
}

void b32::multiply_singles(const b32& op)
{
    split_64 prod;
    prod.w = (uint64_t)get_msb() * (uint64_t)op.get_msb();

    if (prod.a[1] != 0)  {
        num[0] = prod.a[1];
        num.push_back(prod.a[0]);
    } else 
        num[0] = prod.a[0];
}

void b32::multiply_with(const b32& op)
{
    if (is_zero() == true)
        return;

    if (op.is_zero() == true) {
        num.clear();
        return;
    }

    if (op.is_unity() == true) 
        return;

    if (is_unity() == true) {
        op.get_vector(num);
        return;
    }

    if ((op.get_array_size() == 1) && (get_array_size() == 1)) {
        multiply_singles(op);
        return;
    }

    vec32 op_vec;
    op.get_vector(op_vec);
    size_t sz1 = num.size() - 1;
    size_t sz2 = op_vec.size() - 1;
    std::vector<mul_digit> prod_digits;
    
    for (int i = sz1; i >= 0; i--)  {
        if (num[i] == 0)
            continue;
        for (int j = sz2; j >= 0; j--) { 
            if (op_vec[j] == 0)
                continue;
            mul_digit m;
            m.oprnd1 = num[i];
            m.oprnd2 = op_vec[j];
            m.exponent = sz1 - i + sz2 - j;
            m.product.w = 0;
            prod_digits.push_back(m);
        }
    }

    int i = 0;
    __m512i a1,a2;
    while (i < prod_digits.size()) {
        a1 = zero_512;
        a2 = zero_512;
        int j = 0;
        for (;j < 8; j++) {
            if ((i + j) >= prod_digits.size())
                break;
            a1[j] = prod_digits[i + j].oprnd1;
            a2[j] = prod_digits[i + j].oprnd2;
        }

        __m512i mr = _mm512_mul_epu32(a1, a2);

        for (int k = 0; k < j; k++) 
            prod_digits[i + k].product.w = mr[k];

        i += j;
    }
 
    uint32_t max_prod_size = num.size() * op_vec.size() + 2; 
    b32 zero({0});
    std::vector<b32> prod_table(max_prod_size, zero);
    for (auto d:prod_digits) 
        prod_table[d.exponent].add_to(d.product.w);

    for (int i = 0; i < prod_table.size() - 1; i++) { 
        size_t arr_sz = prod_table[i].get_array_size();
        if (arr_sz <= 1)
            continue;
        
        vec32 v;
        prod_table[i].get_vector(v);
        int k = i + 1;
        for (int j = v.size() - 2; j >= 0; j--) 
            prod_table[k++].add_to(v[j]);
        prod_table[i].set_vector({v[v.size() - 1]});
    }
    
    num.clear();
    num.reserve(prod_table.size());
    for (int i = prod_table.size() - 1; i >= 0; i--) 
        num.push_back(prod_table[i].get_msb());
    
    trim_leading_zeros();
}
