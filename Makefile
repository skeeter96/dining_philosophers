CC=gcc
Cflags=
Lflags=

all:dining_philosophers

dining_philosophers:main.c philosopher.c semaphore_lib.c philosopher.h semaphore_lib.h
	$(CC) $(Cflags) -c -o semaphore_lib.o semaphore_lib.c
	$(CC) $(Cflags) -c -o philosopher.o philosopher.c
	$(CC) $(Cflags) -c -o main.o main.c
	$(CC) $(Lflags) -o dining_philosophers semaphore_lib.o philosopher.o main.o

clean: 
	rm -f dining_philosophers semaphore_lib.o philosopher.o main.o
