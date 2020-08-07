CC = gcc
CFLAGS = -g -Wall

TARGET = wc
.PHONY: all clean format check

all: $(TARGET)

clean:
	- rm -f *.o $(TARGET)
format:
	clang-format -i *.c

check: all
	./test.sh

wc: wc.o util.o

util.o: util.c util.h
