CC = gcc
CFLAGS = -Wall -Wextra -std=c11
build: main.c functii_arb.c functii_fisiere.c tema2.c functii_heap.c
	$(CC) $(CFLAGS) -o search_index main.c functii_arb.c functii_fisiere.c tema2.c functii_heap.c

clean:
	rm -f search_index

pack:
	zip -FSr 312CB_DalcuDenisaElena_Tema2 README Makefile *.c *.h