all:
	gcc syllables.c dict.c -o bin/syllables
debug:
	gcc -g  syllables.c dict.c -o bin/syllables 
.PHONY: all
