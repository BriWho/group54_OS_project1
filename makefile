CC := gcc

all: process_queue.o
	$(CC) -o main main.c $^

%.o: process_queue.c
	$(CC) -c $^

.PHONY: clean
clean:
	rm *.o main
