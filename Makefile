CC=gcc
OUT=out

main: main.c
	$(CC) -o $(OUT) pledge.c main.c -lseccomp

clean:
	rm $(OUT)
