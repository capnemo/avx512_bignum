#include <iostream>
#include <set>
#include "b32.h"
#include "b10.h"

using function_map = std::map<char, void (*)(b32&, const b32&)>;
using function_map_iter = function_map::const_iterator;

void usage(const char* pname);

void add(b32& p1, const b32& p2);
void multiply(b32& p1, const b32& p2);
void subtract(b32& p1, const b32& p2);
void divide(b32& p1, const b32& p2);
void get_remainder(b32& p1, const b32& p2);

void run_function(char func, const std::string& arg1, const std::string& arg2, 
                  std::string& result);
bool validate_args(std::vector<std::string>& args);
int run_native(char op, const std::string& s1, const std::string& s2);

int main(int argc, char *argv[])
{
    std::vector<std::string> arg_vec;
    for (int i = 1; i < argc; i++) 
        arg_vec.push_back(std::string(argv[i]));

    if (validate_args(arg_vec) == false) {
        usage(argv[0]);
        return -1;
    }

    std::string ans;
    run_function(arg_vec[0][1], arg_vec[1], arg_vec[2], ans);
    std::cout << ans << std::endl;
    return 0;
}

bool validate_args(std::vector<std::string>& args)
{
    if (args.size() != 3) 
        return false;
 
    std::set<std::string> op_args = {"-a", "-m", "-s", "-d", "-r", "-sign"};
    if ((args.size() == 3) && (op_args.find(args[0]) == op_args.end()))
        return false;

    return true;
}

int run_native(char op, const std::string& s1, const std::string& s2)
{
    int a1 = std::strtol(s1.c_str(), nullptr, 10);
    int a2 = std::strtol(s2.c_str(), nullptr, 10);

    switch (op) {
        case 'a':
            return a1 + a2;
        case 's':
            return a1 - a2;
        case 'm':
            return a1 * a2;
        case 'd':
            return a1 / a2;
        case 'r':
            return a1 % a2;
    }

    return 77777;
}

void run_function(char func, const std::string& arg1, const std::string& arg2, 
                  std::string& result)
{
    b32 op1,op2;
    op1.convert_to_b32(arg1);
    op2.convert_to_b32(arg2);
    
    static function_map func_map = {{'a', add}, 
                                    {'m', multiply}, 
                                    {'s', subtract}, 
                                    {'d', divide},
                                    {'r', get_remainder}};

    function_map_iter f = func_map.find(func);
    if (f == func_map.end())
        return;

    f->second(op1, op2);
    b10::convert_to_b10(op1, result);
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

void get_remainder(b32& p1, const b32& p2)
{
    p1.divide_by(p2);
    b32 r;
    p1.get_remainder(r);
    p1.reset_remainder();
    p1 = r;
}

void usage(const char* pname)
{
    std::cout << "Usage " << pname << " -a|-s|-m|-d <num1> <num2>" << std::endl;
}

