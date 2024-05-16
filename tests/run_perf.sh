#!/usr/bin/bash

echo $#
if [ $# != 4 ]
then
    echo "Usage: $0 <operation> <size1> <size2> <out_file>"
    echo "<operation> = -a|-s|-m|-d|-r"
    echo "<size1> and <size2> are input file sizes"
    echo "<out_file>" is the output file"
    exit
fi

file1=$(./gen_num $2)
file2=$(./gen_num $3)
echo $file1
echo $file2
./file_ops $1 $file1 $file2 $4
