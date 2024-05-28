#include <string>
#include <iostream>
#include <map>
#include "types.h"

#ifndef B32_H
#define B32_H
//multiply -- 
class b32 {

    public:
    b32():num({0}) {}
    b32(const vec32& v, bool s = false):num(v),is_negative(s) {}

    void convert_to_b32(const std::string& b10_num);

    void add_to(const b32& op);
    void add_to(uint64_t adnd);
    void subtract_from(const b32& op);
    void multiply_with(const b32& op);
    void divide_by(const b32& op);

    void set_remainder();
    void get_remainder(b32& rem);
    void reset_remainder();

    void get_vector(vec32& num_vec) const {num_vec = num;}
    void set_vector(const vec32& num_vec) { num = num_vec;}
    bool is_less_than_zero() const { return is_negative; }
    void print_vec() const;
    bool is_zero() const;
    uint32_t get_msb() const;
    uint32_t get_remainder_msb() { return remainder[0]; }

    private:
    void convert_to_b32(const vec8& b10_num, bool under_zero = false);
    void add_singles(const b32& op); 
    void multiply_singles(const b32& op);
    void multiply_with_10();
    void multiply_with_b10_digit(uint8_t digit);
    void subtract_singles(const b32& op);
    void get_base10_num(std::string& str);
    void flip_sign();
    void set_zero();
    void shift_left_digits(uint32_t len);
    void shift_left_array(uint32_t width);
    void shift_left(vec32& digits, uint8_t width);
    void shift_right_array(uint32_t width);
    void shift_right(vec32& digits, uint8_t width);
    void print_bits() const;
    void trim_leading_zeros();
    size_t get_array_size() const;
    size_t get_msb_index(uint32_t n) const;
    int compare_msb(const b32& cmp) const;
    size_t get_array_msb_index() const;
    int compare_abs(const b32& cmp) const;
    bool is_unity() const;
    void set_div_error() {div_error = true;}
    void resolve_signs(const b32& cmp);

    private:
    vec32 num;
    vec32 remainder = {0};
    bool remainder_set = false;
    bool div_error = false;
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
