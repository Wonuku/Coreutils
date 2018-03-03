CC = gcc
WARN = -Wall -Wextra
FL = $(WARN) -march=native -pipe
CFLAGS = $(WARN) $(FL)

binaries=ls rm mv cat

all : $(binaries)

clean :
		rm -f *.o

veryclean :
		rm -f $(binaries)
