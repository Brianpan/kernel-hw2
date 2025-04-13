CC = gcc
CFLAGS = -O1 -g -Wall -Werror

q1: q1.o list.o
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $<  

q2: q2.o
	$(CC) $(CFLAGS) -o $@ $^

q3: q3.o
	$(CC) $(CFLAGS) -o $@ $^

q3_2: q3_2.o
	$(CC) $(CFLAGS) -o $@ $^

q4: q4.o
	$(CC) $(CFLAGS) -o $@ $^

q5: q5.o
	$(CC) $(CFLAGS) -o $@ $^

q6: q6.o
	$(CC) $(CFLAGS) -o $@ $^
