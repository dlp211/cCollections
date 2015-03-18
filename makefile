CC = gcc
CCFLAGS = -g -Wall
DEPS = list.h stack.h sda.h
OBJECTS = test.o list.o stack.o sda.o

all: tests clean

%.o: %.c $(DEPS)
	$(CC) $(CCFLAGS) -c -o $@ $<

tests: $(OBJECTS)
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm *.o
