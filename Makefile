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

build/common.o: src/common.c build
	$(CC) $(CFLAGS) -c -o build/common.o src/common.c -lm -lsempv

build/parallel: src/parallel.c build/common.o
	$(CC) $(CFLAGS) -o build/parallel src/parallel.c build/common.o -lpthread -lm

build/sequencial: src/sequencial.c build/common.o
	$(CC) $(CFLAGS) -o build/sequencial src/sequencial.c build/common.o -lm

build/README.pdf: README.md
	pandoc README.md -o build/README.pdf --template resources/eisvogel.latex --listings


.PHONY: clean
clean:
	rm -rf build

.PHONY: stat
stat:
	ipcs -s

.PHONY: format
format:
	pre-commit run -a

report: build/README.pdf
all: build/parallel build/sequencial
