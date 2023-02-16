# Ⓒ Pirlea Andrei 312CAa 2022-2023

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=my_octave

build: $(TARGETS)

my_octave: my_octave.c
	$(CC) $(CFLAGS) -o my_octave my_octave.c
	
functions: functions.c
	$(CC) $(CFLAGS) -o functions functions.c

pack:
	zip -FSr 312CA_PirleaAndrei_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
