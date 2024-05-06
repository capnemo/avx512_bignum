#include <iostream>
#include <set>
#include "b32.h"
#include "b10.h"

void add(b32& p1, const b32& p2);
void multiply(b32& p1, const b32& p2);
void subtract(b32& p1, const b32& p2);
void divide(b32& p1, const b32& p2);

void usage(const char* pname);

int main(int argc, char *argv[])
{
    if (argc != 4) {
        usage(argv[0]);
        return -1;
    }

    std::vector<std::string> arg_vec{argv[1], argv[2], argv[3]};
    std::set<std::string> op_args = {"-a", "-m", "-s", "-d"};
    if (op_args.find(arg_vec[0]) == op_args.end()) {
        usage(argv[0]);
        return -1;
    }

    std::map<char, void (*)(b32&, const b32&)> func_map = 
                                {{'a', add},
                                 {'m', multiply},
                                 {'s', subtract},
                                 {'d', divide}};
    std::map<char, void (*)(b32&, const b32&)>::const_iterator f = 
    func_map.find(argv[1][1]);
    if (f == func_map.end()) {  
        usage(argv[0]);
        return -1;
    }
           
    b32 op1,op2;
    op1.convert_to_b32(arg_vec[1]);
    op2.convert_to_b32(arg_vec[2]);
    f->second(op1, op2);

    std::string res_str;
    b10::convert_to_b10(op1, res_str);
    std::cout << res_str << std::endl;
}

void add(b32& p1, const b32& p2)
{
    p1.add_to(p2);
}

void multiply(b32& p1, const b32& p2)
{
    p1.multiply_with(p2);
}

void subtract(b32& p1, const b32& p2)
{
    p1.subtract_from(p2);
}

void divide(b32& p1, const b32& p2)
{
    p1.divide_by(p2);
}

void usage(const char* pname)
{
    std::cout << "Usage " << pname << " -a|-s|-m|-d <num1> <num2>" << std::endl;
}
