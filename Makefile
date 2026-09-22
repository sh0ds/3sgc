CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:src/%.c=build/%.o)

build/statiq: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

.PHONY: clean
