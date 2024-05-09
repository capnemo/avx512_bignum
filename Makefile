OBJS:=bin/b10.o bin/b32_util.o bin/b32_add.o bin/b32_multiply.o bin/b32_subtract.o bin/b32_divide.o
SRCS:=src/b10.cpp src/b32_util.cpp src/b32_add.cpp src/b32_multiply.cpp src/b32_subtract.cpp src/b32_divide.cpp
INCL:=incl/b10.h incl/b32.h incl/types.h
TEST_BINS:=tests/conv_drv tests/operator_drv
TEST_SRC=:=tests/conv_drv.cpp tests/operator_drv.cpp
CC=g++ -c -Iincl -mavx512bw
CC2=g++ -Iincl 

debug: CC += -ggdb -Og
debug: CC2 += -ggdb -Og
debug: all

release: CC += -Os
release: CC2 += -Os
release: all

all:binX bin/b32.a tests

bin/%.o:src/%.cpp 
	$(CC) $^ -o $@

binX:
	mkdir -p bin

bin/b32.a:$(OBJS)
	ar r bin/b32.a $(OBJS)

tests: tests/gen_num tests/conv_drv tests/operator_drv bin/b32.a

tests/gen_num:tests/gen_num.cpp
	$(CC2) $^ -o $@

tests/operator_drv:tests/operator_drv.cpp bin/b32.a
	$(CC2) $^ -o $@
	
tests/conv_drv:tests/conv_drv.cpp bin/b32.a
	$(CC2) $^ -o $@

clean:
	rm -rf bin tests/operator_drv tests/gen_num tests/conv_drv
