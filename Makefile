CC = gcc
CFLAGS = -g -Wall

TARGET = wc
.PHONY: all clean format check

all: $(TARGET)

clean:
	- rm -f *.o $(TARGET) test
format:
	clang-format -i *.c

check: all test
	./test

wc: wc.o util.o

util.o: util.c util.h
