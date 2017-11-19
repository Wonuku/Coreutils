CC = gcc
WARN = -Wall -Wextra
FL = $(WARN) -march=native -pipe
CFLAGS = $(WARN) -c $(FL)

all : ls rm mv cat

ls :
		$(CC) $(FL) -o ls ls.c

mv :
		$(CC) $(FL) -o mv mv.c

rm :
		$(CC) $(FL) -o rm rm.c

clean :
		rm -f *.o
