# Ⓒ Pirlea Andrei 312CAa 2022-2023

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c functions.c
	$(CC) $(CFLAGS) *.c -lm -o image_editor

pack:
	zip -FSr 312CA_PirleaAndrei_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
