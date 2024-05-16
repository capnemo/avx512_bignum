#include "b32.h"
#include "b10.h"

//To Be Considered. When shifted_op is shifted by n, consider filling
// the bits from n to n - r with shifted op.
//for x in n - r to r shift op and add to shifted_op. r is the size of op
//Test it out by hand. For example, This will reduce the number of 
//iterations for 1111111111b2/2

/*
 * Computes *this /= op.
 * IN:op
 */

void b32::divide_by(const b32& op)
{
    if (op.is_unity() == true) 
        return;

    if (op.is_zero() == true) {
        set_div_error();
        return;
    }

    if (is_zero() == true) 
        return;

    resolve_signs(op);

    if ((get_array_size() == 1) && (op.get_array_size() == 1)) {
        remainder.clear();
        remainder.push_back(get_msb() % op.get_msb());
        num[0] = get_msb() / op.get_msb();
        return;
    }

    int64_t df = compare_abs(op);
    if (df == 0) {
        num.clear();
        num.push_back(1);
        return;
    }

    if (df < 0) {
        get_vector(remainder);
        num.clear();
        num.push_back(0);
        return;
    }

    b32 quot({0}, is_negative);
    while (this->compare_abs(op) >= 0) {
        uint32_t shift_width = get_array_msb_index() - 
                               op.get_array_msb_index();
        b32 shifted_op = op;
        shifted_op.shift_left_array(shift_width);
        if (this->compare_abs(shifted_op) < 0)  {
            shifted_op.shift_right_array(1);
            shift_width--;
        }
        this->subtract_from(shifted_op);
        b32 current_quot({1});
        current_quot.shift_left_array(shift_width);
        quot.add_to(current_quot);
    }
    
    remainder = num;
    quot.get_vector(num);  
}

/*
 * Resets the remainder
 */
void b32::reset_remainder()
{
    remainder = {0};
}

/*
 * Returns the remainder
 * OUT:rem
 */
void b32::get_remainder(b32& rem) 
{
    rem = b32(remainder, is_negative);
}
