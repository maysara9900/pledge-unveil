CC=gcc
OUT=out

main: main.c
	$(CC) -o $(OUT) unveil.c pledge.c main.c -lseccomp

clean:
	rm $(OUT)
