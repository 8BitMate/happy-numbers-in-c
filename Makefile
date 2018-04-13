CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3
VFLAGS = --track-origins=yes

happy: happy-numbers.c
	$(CC) $(CFLAGS) $^ -o $@

checkhappy: happy
	valgrind $(VFLAGS) ./happy 10000000
