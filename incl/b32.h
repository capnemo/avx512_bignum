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

    void add_to(const b32& op);
    void add_to(uint64_t adnd);
    void multiply_with(const b32& op);

    void get_vector(vec32& num_vec) const {num_vec = num;}
    void set_vector(const vec32& num_vec) { num = num_vec;}
    bool is_less_than_zero() const { return is_negative; }

    private:
    void add_singles(const b32& op);
    void multiply_singles(const b32& op);
    void shift_left_digits(uint32_t len);
    bool is_zero() const;
    void print_vec() const;
    void get_base10_num(std::string& str);
    void convert_to_b32(const std::string& b10_num);
    void convert_to_b32(const vec8& b10_num);
    void flip_sign();
    void set_zero();
    void shift_left_array(uint32_t width);
    void shift_left(vec32& digits, uint8_t width);
    void shift_right_array(uint32_t width);
    void shift_right(vec32& digits, uint8_t width);
    void print_bits() const;
    void trim_leading_zeros();
    size_t get_array_size() const;
    size_t get_msb_index(uint32_t n) const;
    uint32_t get_msb() const;
    int compare_msb(const b32& cmp) const;
    size_t get_array_msb_index() const;
    int compare_abs(const b32& cmp) const;
    bool is_unity() const;

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
