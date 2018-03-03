CC = clang
WARN = -Wall -Wextra
FL = $(WARN) -march=native -pipe
CFLAGS = $(WARN) $(FL)

binaries=src/ls src/rm src/mv src/cat src/yes

all : $(binaries)
	mv $(binaries) .

clean :
		rm -f *.o

veryclean :
		rm -f $(binaries)
