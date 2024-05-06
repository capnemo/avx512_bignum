#include <string>
#include <iostream>
#include <map>
#include "types.h"

#ifndef B32_H
#define B32_H
//multiply -- 
class b32 {

    public:
    b32() {}
    b32(const vec32& v, bool s = false):num(v),is_negative(s) {}
    
    void get_vector(vec32& num_vec) const {num_vec = num;}
    void set_vector(const vec32& num_vec) { num = num_vec;}
    bool is_less_than_zero() const { return is_negative; }

    private:
    vec32 num;
    bool is_negative = false;
    static constexpr __m512i zero_512 = {0, 0, 0, 0, 0, 0, 0, 0};
};

typedef union {
    uint32_t a[2];
    uint64_t w;
} split_64;

typedef struct {
    uint32_t oprnd1;
    uint32_t oprnd2;
    uint32_t exponent;
    split_64 product;
} mul_digit;

#endif //B32_H
