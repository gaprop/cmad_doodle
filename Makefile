
LDFLAGS=-lcsv

all: main csv.c
# all: main.c csv.c
	# gcc csv.c main.c -o main

run: all
	./main

