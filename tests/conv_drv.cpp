#include <iostream>
#include "b32.h"
#include "b10.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return -1;
    }

    std::string in_num(argv[1]);

    b32 bnum;
    bnum.convert_to_b32(in_num);

    std::string out_num;
    b10::convert_to_b10(bnum, out_num);

    std::cout << ((out_num == in_num) ? "pass":"fail") << std::endl;
    std::cout << in_num << std::endl;
    bnum.print_vec();
    std::cout << out_num << std::endl;
}

#if 0
int main(int argc, char *argv[])
{
    b32 b({3,4});
    b32 a({1,0,0,0,0,0,0});
    a.add_to(b);
    a.print_vec();
}

int main(int argc, char *argv[])
{
    b32 a({1,1,1,1,1,1,1,1,1});
    b32 b({1,1,1,1,1,1,1,1,1});
    a.multiply_with(b);
    a.print_vec();
}
#endif
