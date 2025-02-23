NAME := main

OBJ = main.o trie.o pstring.o

OUTPUT_OPTION = -o obj/$@

vpath %.c src/
vpath %.o obj/

CFLAGS := -Wall -Wextra -Werror -O3
CPPFLAGS := -I include/

main: $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

