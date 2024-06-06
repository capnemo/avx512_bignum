#include "b10.h"
 
void b32_to_string(uint32_t b32_num, std::string& b10_str);
namespace b10 {   
    /* Converts a b32 object to a base 10 string using b32::divide_by
     * Immeasurably inefficient. Optimizing divide_by might make it better
     * IN: num
     * OUT: n10
     */
    void convert_to_b10_divide(const b32& num, std::string& n10)
    {
        b32 nmtor = num;
        b32 dntor({10});
        while(nmtor.is_zero() == false) {
            nmtor.divide_by(dntor);
            n10.insert(n10.begin(), nmtor.get_remainder_msb() + '0');
        }
    }

    /* Converts a b32 object to a base 10 string using b32::divide_by
     * About 9x faster than convert_to_b10_divide but not as fast as
     * convert_to_b10
     * IN: num
     * OUT: n10
     */
    void convert_to_b10_v3(const b32& num, std::string& n10)
    {
        b32 nmtor = num;
        b32 dntor({1'000'000'000});
        while(nmtor.is_zero() == false) {
            nmtor.divide_by(dntor);
            uint32_t rem = nmtor.get_remainder_msb();

            std::string rem_str;
            b32_to_string(rem, rem_str);

            if (nmtor.is_zero() == false) 
                rem_str.insert(0, 9 - rem_str.size(), '0');

            n10.insert(0, rem_str);
        }
    }
}

void b32_to_string(uint32_t b32_num, std::string& b10_str)
{
    if (b32_num == 0) {
        b10_str = "0";
        return;
    }   
    
    b10_str = ""; 
    while (b32_num > 0) {
        b10_str.insert(0, 1, b32_num % 10 + '0');
        b32_num /= 10; 
    }   
}
