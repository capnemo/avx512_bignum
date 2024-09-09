#include <iostream>
#include <cstdlib>
#include "b32_2x9.h"

uint64_t* stuff_vector(uint32_t len);
bool compare(uint64_t* n1, uint64_t* n2, uint32_t sz);
int main(int argc, char* argv[])
{
    b32_2x9 x;
    uint64_t *p = stuff_vector(37);
    x.set_number(p, 37);
    x.print_vec();
    printf("-------------------------------\n");
    x.shift_left(11);
    x.print_vec();
    printf("-------------------------------\n");

    x.shift_right(11);
    x.print_vec();
    uint64_t *rp = new uint64_t[x.get_actual_size()];
    x.get_copy(&rp);
    if (compare(p, rp, 37) == false)
        std::cout << "Numbers are NOT equal" << std::endl;
    else
        std::cout << "Numbers are equal" << std::endl;
    delete [] rp;
    delete [] p;
}

uint64_t* stuff_vector(uint32_t len)
{
    srandom(time(nullptr));
    uint64_t *ptr = new uint64_t[len];
    for (int i = 0; i < len; i++)  
        ptr[i] = random();

    return ptr;

}

bool compare(uint64_t* n1, uint64_t* n2, uint32_t sz)
{
    for (int i = 0; i < sz; i++) 
        if (n1[i] != n2[i]) {
            std::cout << i << std::endl;
            return false;
        }

    return true;
}
