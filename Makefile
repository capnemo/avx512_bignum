OBJS:=bin/b10.o
SRCS:=src/b10.cpp 
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


