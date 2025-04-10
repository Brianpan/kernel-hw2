CC = gcc
CFLAGS = -O1 -g -Wall -Werror

q1: q1.o list.o
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $<  

q2: q2.o
	$(CC) $(CFLAGS) -o $@ $^