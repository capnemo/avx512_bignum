#include <stdio.h>
#include <stdint.h>

void new_carry(uint64_t* num);
void print_num(uint64_t* num);
int main(int argc, char *argv[])
{
    uint64_t a[] = {1,0,0,0,0,0,0,-1};
    print_num(a);
    new_carry(a);
    print_num(a);

    uint64_t b[] = {6,5,4,3,2,1,0,-1};
    print_num(b);
    new_carry(b);
    print_num(b);

}

void print_num(uint64_t* num)
{
    for (int i = 0; i < 8; i++)
        printf("%ld ", num[i]);
    printf("\n");
}
