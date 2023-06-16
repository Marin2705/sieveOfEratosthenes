CC = gcc
CFLAGS =  \
	-Wredundant-decls      \
	-Wformat-security      \
	-Wformat-signedness    \
	-Wextra                \
	-Wall                  \
	-Wshadow               \
	-O0					   \
	-Wdouble-promotion     \
	-Wfloat-equal          \
	-Wpedantic             \
	-pedantic-errors       \
	-fverbose-asm          \
	-gdwarf

ifneq ($(OS),Windows_NT)
    CCFLAGS += -fsanitize=leak
endif

.DEFAULT_GOAL := all

build:
	mkdir -p build

build/parallel: src/parallel.c build
	$(CC) $(CFLAGS) -o build/parallel src/parallel.c -lpthread

build/sequencial: src/sequencial.c build
	$(CC) $(CFLAGS) -o build/sequencial src/sequencial.c -lm

build/README.pdf: README.md
	pandoc README.md -o build/README.pdf --template resources/eisvogel.latex --listings


.PHONY: clean
clean:
	rm -rf build

.PHONY: format
format:
	pre-commit run -a

report: build/README.pdf
all: build/parallel build/sequencial
