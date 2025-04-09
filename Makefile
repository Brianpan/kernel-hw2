CC = gcc
CFLAGS = -O1 -g -Wall -Werror

q1: q1.c
	$(CC) $(CFLAGS) -o q1 q1.c

