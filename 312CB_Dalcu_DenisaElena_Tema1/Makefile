CC = gcc
CFLAGS = -Wall -Wextra -g
build: main.c functii.c structuri.h
	$(CC) $(CFLAGS) -o tema1 main.c functii.c
clean:
	rm -f tema1 *.o tema1.out
pack:
	zip -FSr 312CB_Dalcu_DenisaElena_Tema1 README Makefile *.c *.h
