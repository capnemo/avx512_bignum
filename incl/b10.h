#include "types.h"
#include "b32.h"

#ifndef B10_H
#define B10_H

/*
 * Of all the convert_to_b10 functions, 
 * convert_to_b10(const b32& num, std::string& n10); is the most efficient.
 * The others need divide_by to get more efficient
 */
namespace b10 {
    void convert_to_b10(const vec32& num, vec8& n10);
    void convert_to_b10(const b32& num, std::string& n10);
    void convert_to_b10_divide(const b32& num, std::string& n10);
    void convert_to_b10_v3(const b32& num, std::string& n10);
}
#endif
