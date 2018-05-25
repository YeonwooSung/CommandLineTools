FLAGS = -Wall -Wextra -std=c99

all: print.o ls.o ls-program

clean:
    rm print.o ls-program ls.o

ls-program: main.c ls.o print.o ls.h
    gcc -o ls-program main.c print.o ls.o -g $(FLAGS)

ls.o: ls.c
    gcc -o ls.o ls.c -c $(FLAGS)

print.o: print.c
    gcc -o print.o print.c -c $(FLAGS)