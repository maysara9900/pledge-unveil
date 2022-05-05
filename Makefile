CC=gcc

main: main.c
	$(CC) -o out pledge.c main.c -lseccomp

clean:
	rm out
