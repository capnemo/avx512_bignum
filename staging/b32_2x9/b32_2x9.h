#include "types.h"
#include "aligned_allocator.h"

class b32_2x9 {
    public:
    void set_number(uint64_t* n, uint32_t sz);
    void shift_left(uint32_t shift_len);
    void shift_right(uint32_t shift_len);
    uint32_t get_actual_size();
    void get_copy(uint64_t** buff);
    void print_vec();
    ~b32_2x9();
    
    private:
    void find_leading_zeros();
    void reset_number(int32_t new_len);

    private:
    au64_vec num = {nullptr};
    uint32_t leading_zeros;
    aligned_allocator allctr;
};
