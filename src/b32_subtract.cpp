#include "b32.h"
#include "b10.h"


/*
 * Computes *this -= op when *this and op are <= 2^32 - 1 
 * IN: op 
 */
void b32::subtract_singles(const b32& op)
{
    int64_t diff = (int64_t)get_msb() - (int64_t)op.get_msb();
    
    if (diff < 0)  
        diff *= -1;

    num[0] = diff;
}

/*
 * Computes *this -= op when op b32 object
 * IN: op 
 */
void b32::subtract_from(const b32& op)
{
    if (op.is_zero() == true)
        return;

    vec32 op_vec;
    op.get_vector(op_vec);
    if (is_zero() == true) {
        num = op_vec;
        flip_sign();
        return;
    }

    bool op_sign = !op.is_less_than_zero();
    if (is_negative == op_sign) {
        b32 new_op(op_vec, op_sign);
        add_to(new_op);
        return;
    }

    int diff = compare_abs(op);
    if (diff == 0) {
        set_zero();
        return;
    }

    if (diff < 0)
        flip_sign();

    if ((op.get_array_size() == 1) && (get_array_size() == 1)) {
        subtract_singles(op);
        return;
    }

    vec32* lg_ptr = &num;
    vec32* sm_ptr = &op_vec;
    if (diff < 0) {
        lg_ptr = &op_vec;
        sm_ptr = &num;
    }

    size_t sz_diff = lg_ptr->size() - sm_ptr->size();
    std::vector<int64_t> diff_vec;
    diff_vec.reserve(sm_ptr->size());
    __m512i a1,a2;
    a1 = zero_512;
    a2 = zero_512;
    size_t m512_in = 0;
    size_t cm_in = 0;
    while (cm_in < sm_ptr->size()) {
        a1[m512_in] = (*lg_ptr)[sz_diff + cm_in];
        a2[m512_in] = (*sm_ptr)[cm_in];
        m512_in++;
        cm_in++;
        if ((m512_in == 8) || (cm_in == sm_ptr->size()))  {
            __m512i sum = _mm512_sub_epi64(a1, a2);
            for (int i = 0; i < m512_in; i++)  
                diff_vec.push_back(sum[i]);
            m512_in = 0;
            a1 = zero_512;
            a2 = zero_512;
        }
    }
    
    size_t df_in = diff_vec.size() - 1;
    size_t lg_in = lg_ptr->size() - 1;
    uint32_t carry = 0;
    uint64_t compliment = 0x100000000;
    for (int offst = 0; offst < diff_vec.size(); offst++) {
        int64_t num64 = diff_vec[df_in - offst] - carry;
        carry = 0;
        if (num64 < 0) {
            num64 += compliment;
            carry = 1;
        }
        (*lg_ptr)[lg_in - offst] = num64;
    }

    int st = lg_ptr->size() - diff_vec.size() - 1;
    for (int p = st; p >= 0; p--) {
        int64_t num64 = (*lg_ptr)[p] - carry;
        carry = 0;
        if (num64 < 0) {
            num64 += compliment;
            carry = 1;
        }
        (*lg_ptr)[p] = num64;
        if (carry == 0)
            break;
    }

    num = *lg_ptr;
    trim_leading_zeros();
}

