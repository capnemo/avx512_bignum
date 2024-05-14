#include <iostream>
#include <fstream>
#include <set>
#include <ctime>
#include "b32_operations.h"
#include "b10.h"

using function_map = std::map<char, void (*)(const b32&, const b32&, b32&)>;
using function_map_iter = function_map::const_iterator;

void usage(const char* pname);

void add(const b32& p1, const b32& p2, b32& r);
void multiply(const b32& p1, const b32& p2, b32& r);
void subtract(const b32& p1, const b32& p2, b32& r);
void divide(const b32& p1, const b32& p2, b32& r);

void run_function(char func, const std::string& arg1, const std::string& arg2, 
                  std::string& result);
void read_file(const char* fname, std::string& cont);
void write_file(const char* fname, std::string& cont);
void print_time(const std::string& pref);

int main(int argc, char *argv[])
{
    if (argc != 5) {
        usage(argv[0]);
        return -1;
    }

    std::string arg1, arg2, sol;
    read_file(argv[2], arg1);
    if (arg1.size() == 0) {
        std::cout << "error reading file 1" << std::endl;
        return 0;
    }

    read_file(argv[3], arg2);
    if (arg2.size() == 0) {
        std::cout << "error reading file 2" << std::endl;
        return 0;
    }

    run_function(argv[1][1], arg1, arg2, sol);
    write_file(argv[4], sol);
}

void read_file(const char* fname, std::string& cont)
{
    std::ifstream fhandle(fname);
    if (fhandle)
        fhandle >> cont;
}

void write_file(const char* fname, std::string& cont)
{
    std::ofstream fhandle(fname);
    if (fhandle)
        fhandle << cont;
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
                                    {'d', divide}};

    function_map_iter f = func_map.find(func);
    if (f == func_map.end())
        return;

    b32 res;
    f->second(op1, op2, res);
    b10::convert_to_b10(res, result);
}

void add(const b32& p1, const b32& p2, b32& r)
{
    b32_operations::add(p1, p2, r);
}

void multiply(const b32& p1, const b32& p2, b32& r)
{
    b32_operations::multiply(p1, p2, r);
}

void subtract(const b32& p1, const b32& p2, b32& r)
{
    b32_operations::subtract(p1, p2, r);
}

void divide(const b32& p1, const b32& p2, b32& r)
{
    b32_operations::divide(p1, p2, r);
}

void usage(const char* pname)
{
    std::cout << "Usage " << pname << " -a|-s|-m|-d <file1> <file2> <file3>" << std::endl;
    std::cout << "file1 and file2 are input operands. file3 will contain the operation result" << std::endl;
}

