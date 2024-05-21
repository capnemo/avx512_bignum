#include <iostream>
#include <fstream>
#include <ctime>
#include "b32.h"
#include "b10.h"

void usage(const char* pname);
void read_file(const char* file_name, std::string& num);
void write_file(const std::string& file_name, std::string& num);
int main(int argc, char *argv[])
{
    if ((argc != 2) && (argc != 3)) {
        usage(argv[0]);
        return -1;
    }

    std::string input_number;
    std::string second_arg = argv[1];
    bool file_out = false;
    if (argc == 3) {
        if (std::string(argv[1]) != second_arg) {
            usage(argv[0]);
            return -1;
        }
        read_file(argv[2], input_number);
        file_out = true;
    }

    if (file_out == false) 
        input_number = second_arg;
        
    b32 bnum;
    time_t st = time(0);
    bnum.convert_to_b32(input_number);
    time_t con = time(0);

    std::string out_num;
    b10::convert_to_b10(bnum, out_num);
    time_t fin = time(0);

    std::cout << "b10 -> b32 " << (con - st) << std::endl;
    std::cout << "b32 -> b10 " << (fin - con) << std::endl;
    std::cout << ((out_num == input_number) ? "pass":"fail") << std::endl;
    if (file_out == false) {
        std::cout << input_number << std::endl;
        bnum.print_vec();
        std::cout << out_num << std::endl;
    } else {
        write_file("num.b10.in", input_number);
        write_file("num.b10.out", out_num);
        vec32 out_vec;
        bnum.get_vector(out_vec);
        std::string str_b32;
        for (auto m:out_vec)
            str_b32 += (std::to_string(m) + ' ');
        write_file("num.b32.out", str_b32);
    }
}

void write_file(const std::string& file_name, std::string& num)
{
    std::ofstream fhandle(file_name.c_str());
    if (fhandle) 
        fhandle << num;
}

void read_file(const char* file_name, std::string& num)
{
    std::ifstream fhandle(file_name);
    if (fhandle) 
        fhandle >> num;
}

void usage(const char* pname)
{
    std::cout << "Usage: " << pname << "<number string>" << std::endl;
    std::cout << "Usage: " << pname << " -f <file_name>" << std::endl;
    std::cout << "If -f is the first argument,\n"
              << "num.b10.in contains the input number,\n" 
              << "num.b10.out contains the output number in base 10\n" 
              << "and num.b32.out contains the output number in " 
              << "base 32" << std::endl;
}
