#include <stdlib.h>
#include <strings.h>
#include "aligned_allocator.h"

void aligned_allocator::allocate(au64_vec& in_vec, uint32_t sz, uint32_t& pad)
{
    pad = pad_to_8(sz);
    int alloc_size = sz + pad;
    uint64_t *ptr;
    uint32_t alloc_size_bytes = alloc_size * sizeof(uint64_t);
    list_iter lt = ready_list.find(alloc_size);
    if ((lt != ready_list.end()) && (lt->second.size() != 0)) {
        ptr = lt->second.front();
        lt->second.pop();
    } else 
        posix_memalign((void **)&ptr, 64, alloc_size_bytes);

    bzero(ptr, alloc_size_bytes);
    vectorize(ptr, alloc_size, in_vec);
}

void aligned_allocator::deallocate(au64_vec& in_vec)
{
    uint64_t *ptr = in_vec[0];
    uint32_t sz = in_vec.size() * register_length;
    list_iter lt = ready_list.find(sz);
    if (lt == ready_list.end()) {
        std::queue<uint64_t*> nq;
        nq.push(ptr);
        ready_list.insert(table_pair(sz, nq));
        return;
    }
    lt->second.push(ptr);
}

uint32_t aligned_allocator::pad_to_8(uint32_t n)
{
    uint32_t r = n % register_length;
    return (r == 0) ? 0:register_length - r;
}

void aligned_allocator::vectorize(uint64_t* buff, uint32_t ln, au64_vec& in_vec)
{
    in_vec.clear();
    uint64_t *cp = buff;
    for (int i = 0; i < ln/register_length; i++)  {
        in_vec.push_back(cp);
        cp += register_length;
    }
}

void aligned_allocator::release_all()
{
    for (auto& m:ready_list) {
        while (m.second.size() != 0) {
            free(m.second.front());
            m.second.pop();
        }
    }
}
