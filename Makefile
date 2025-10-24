all: main

main: src/main.o
	gcc -o bin/main src/main.o

main.o: src/main.c
	gcc -c src/main.c

clean:
	rm -f src/main src/main.o