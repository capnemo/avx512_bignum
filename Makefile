OBJS:=bin/b10.o bin/b32_util.o bin/b32_add.o bin/b32_multiply.o bin/b32_subtract.o bin/b32_divide.o
SRCS:=src/b10.cpp src/b32_util.cpp src/b32_add.cpp src/b32_multiply.cpp src/b32_subtract.cpp src/b32_divide.cpp
INCL:=incl/b10.h 

CC=g++ -c -Iincl -mavx512bw
CC2=g++ -Iincl

debug: CC += -ggdb
debug: CC2 += -ggdb
debug: all

release: CC += -Ofast
release: CC2 += -Ofast
release: all

all:binX bin/b32.a 

bin/%.o:src/%.cpp
	$(CC) $^ -o $@

binX:
	mkdir -p bin

bin/b32.a:$(OBJS)
	ar r bin/b32.a $(OBJS)

clean:
	rm -rf bin

