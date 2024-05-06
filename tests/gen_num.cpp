#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <cstdint>
#include <stdlib.h>
#include <vector>

void usage(char* pname);
int main(int argc, char *argv[])
{
    if ((argc != 2) && (argc != 3)) {
        usage(argv[0]);
        return -1;
    }

    std::string arg2("-x");
    if ((argc == 3)  && (argv[1] != arg2)) {
        usage(argv[0]);
        return -1;
    }

    size_t num_sz = 0;
    if (argc == 3) {
        uint32_t exp = std::strtoull(argv[2], nullptr, 10);
        if (exp > 9) {
            usage(argv[0]);
            return -1;
        }
        num_sz = pow(10, exp);
    }
    
    if (num_sz == 0) 
        num_sz = std::strtoull(argv[1], nullptr, 10);

    std::vector<uint8_t> num_list(num_sz, 0);
    srandom(time(0));

    int t = 0;
    while (t == 0)
        t = random()%10;
    num_list[0] = t + '0';

    for (int i = 1; i < num_sz; i++) 
        num_list[i] = random()%10 + '0';

    char file_name[] = "/tmp/gnum-XXXXXX";
    int file_handle = mkstemp(file_name);
    write(file_handle, num_list.data(), num_list.size());
    close(file_handle);
    std::cout << file_name << std::endl;
}

void usage(char* pname)
{
    std::cout << "To generate a number of length num" << std::endl;
    std::cout << "Usage: " << pname << " <num>" << std::endl;
    std::cout << "To generate a number whose length is a power of 10" 
              << std::endl;
    std::cout << "The power cannot exceed 9" << std::endl;
    std::cout << "Usage: " << pname << " [-x] " << " <power>" << std::endl;
}
