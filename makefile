CC = gcc
CCFLAGS = -g -Wall
DEPS = list.h
OBJECTS = test.o list.o

all: tests clean

%.o: %.c $(DEPS)
	$(CC) $(CCFLAGS) -c -o $@ $<

tests: $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm *.o
