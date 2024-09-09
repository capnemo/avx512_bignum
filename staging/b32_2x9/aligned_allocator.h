#include <vector>
#include <queue>
#include <map>
#include <cstdint>
#include "types.h"

using list_iter = std::map<uint32_t, std::queue<uint64_t*>>::iterator;
using table_pair = std::pair<uint32_t, std::queue<uint64_t*>>;

class aligned_allocator {
    public:
    void allocate(au64_vec& in_vec, uint32_t sz, uint32_t& pad);
    void deallocate(au64_vec& in_vec);
    void release_all();

    private:
    void vectorize(uint64_t* buff, uint32_t ln, au64_vec& in_vec);
    uint32_t pad_to_8(uint32_t n);


    private:
    std::map<uint32_t, std::queue<uint64_t*>> ready_list;
};
