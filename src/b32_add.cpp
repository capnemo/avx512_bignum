#include "b32.h"
#include "b10.h"
#include <string.h>
#include <bitset>

void b32::add_to(uint64_t adnd) 
{
    split_64 t;
    t.w = adnd;
    vec32 digits({t.a[0]});
    if (t.a[1] != 0) 
        digits.insert(digits.begin(), t.a[1]);
    b32 b(digits);
    add_to(b);
}

void b32::add_singles(const b32& op)
{
    split_64 sum;
    sum.w = (uint64_t)get_msb() + (uint64_t)op.get_msb();
    
    if (sum.a[1] != 0) {
        num[0] = sum.a[1];
        num.push_back(sum.a[0]);
    } else 
        num[0] = sum.a[0];
}

void b32::add_to(const b32& op)
{
    if (op.is_zero() == true)
        return;
   
    vec32 op_vec;
    op.get_vector(op_vec);
    if (is_zero() == true) {
        num = op_vec;
        return;
    }

    if (is_less_than_zero() != op.is_less_than_zero()) {
        b32 new_op = op;
        new_op.flip_sign();
        subtract_from(new_op);
        return;
    }

    if ((op.get_array_size() == 1) && (get_array_size() == 1)) {
        add_singles(op);
        return;
    }

    vec32* lg_ptr;
    vec32* sm_ptr;
    if (num.size() < op_vec.size()) {
        lg_ptr = &op_vec;
        sm_ptr = &num;
    } else {
        lg_ptr = &num;
        sm_ptr = &op_vec;
    }

    size_t sz_diff = lg_ptr->size() - sm_ptr->size();
    std::vector<uint64_t> sum_vec;
    sum_vec.reserve(sm_ptr->size());
    __m512i a1 = zero_512, a2 = zero_512;
    size_t m512_in = 0;
    size_t cm_in = 0;
    while (cm_in < sm_ptr->size()) {
        a1[m512_in] = (*lg_ptr)[sz_diff + cm_in];
        a2[m512_in] = (*sm_ptr)[cm_in];
        m512_in++;
        cm_in++;
        if ((m512_in == 8) || (cm_in == sm_ptr->size()))  {
            __m512i sum = _mm512_add_epi64(a1, a2);
            for (int i = 0; i < m512_in; i++)  
                sum_vec.push_back(sum[i]);
            m512_in = 0;
            a1 = zero_512;
            a2 = zero_512;
        }
    }

    size_t sm_in = sum_vec.size() - 1;
    size_t lg_in = lg_ptr->size() - 1;
    uint32_t carry = 0;
    for (int offst = 0; offst < sum_vec.size(); offst++) {
        split_64 n;
        n.w = sum_vec[sm_in - offst] + carry;
        (*lg_ptr)[lg_in - offst] = n.a[0];
        carry = n.a[1];
    }

    int st = lg_ptr->size() - sum_vec.size() - 1;
    for (int p = st; p >= 0; p--) {
        split_64 n;
        n.w = (*lg_ptr)[p] + carry;
        (*lg_ptr)[p] = n.a[0];
        carry = n.a[1];
        if (carry == 0)
            break;
    }

    if (carry != 0) 
        lg_ptr->insert(lg_ptr->begin(), carry);

    num = *lg_ptr;
}
