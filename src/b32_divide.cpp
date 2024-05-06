#include "b32.h"
#include "b10.h"

//All ops have to consider the case of a single element in *this and the
//argument


//To Be Considered. When shifted_op is shifted by n, consider filling
// the bits from n to n - r with shifted op.
//for x in n - r to r shift op and add to shifted_op. r is the size of op
//Test it out by hand. For example, This will reduce the number of 
//iterations for 2^32/2

// All ops.. make sure all of them handle cases of unity and zero

void b32::divide_by(const b32& op)
{
    if (op.is_unity() == true) 
        return;

    if (op.is_zero() == true) {
        set_div_error();
        return;
    }

    int64_t df = compare_abs(op);
    if (df == 0) {
        num.clear();
        num.push_back(1);
        return;
    }

    if (df < 0) {
        num.clear();
        num.push_back(0);
        return; //handle the remainder
    }

    b32 dvsr({0});
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
        b32 current_dvsr({1});
        current_dvsr.shift_left_array(shift_width);
        dvsr.add_to(current_dvsr);
    }
    
    //At this point, num is the remainder
    dvsr.get_vector(num);  
}
