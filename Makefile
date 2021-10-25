SRCS = main.c schelling.c gifenc.c schelling_gif.c
OBJS = $(SRCS:%.c=%.o)
CC = gcc
PROG = Schelling

CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99 -Og -g
LDFLAGS = -lm

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.PHONY: clean run

clean:
	$(RM) $(OBJS) $(PROG)

run: $(PROG)
	./$(PROG)

runwin32: $(PROG)
	$(PROG).exe
