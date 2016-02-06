all:
	gcc syllables.c lib/dict.c -o bin/syllables
	gcc matcher.c -o bin/matcher
debug:
	gcc -g  syllables.c lib/dict.c -o bin/syllables
.PHONY: all
