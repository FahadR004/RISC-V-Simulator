CC = gcc
CFLAGS = -Wall -Wextra -std=c11 

TARGET = risc_v_sim

SRCS = main.c cpu.c parser.c utils.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c cpu.h parser.h
	$(CC) $(CFLAGS) -c main.c

cpu.o: cpu.c cpu.h
	$(CC) $(CFLAGS) -c cpu.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)