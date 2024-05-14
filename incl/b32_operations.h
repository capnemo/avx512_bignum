#include "b32.h"

#ifndef B32_OPERATIONS
#define B32_OPERATIONS

namespace b32_operations {
    void add(const b32& a, const b32& b, b32& result)
    {
        result = a;
        result.add_to(b);
    }

    void subtract(const b32& a, const b32& b, b32& result)
    {
        result = a;
        result.subtract_from(b);
    }

    void multiply(const b32& a, const b32& b, b32& result)
    {
        result = a;
        result.multiply_with(b);
    }

    void divide(const b32& a, const b32& b, b32& result)
    {
        result = a;
        result.divide_by(b);
    }
}

#endif //B32_OPERATIONS
