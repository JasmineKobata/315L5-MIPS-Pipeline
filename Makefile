CC = gcc

CFLAGS = -Wall -g

LD = gcc

LDFLAGS =

all:	lab5
	
lab5: lab5.o
	$(LD) $(LDFLAGS) -o lab5 lab5.o -std=gnu89

lab5.o: load_testcase.c
	$(CC) $(CFLAGS) -c -o lab5.o load_testcase.c -std=gnu89
