EXEC = duckcoincoin

CC = clang
CFLAGS = -std=c11 -Wextra -Wall -pedantic -pipe
LDFLAGS = -lm
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

ifeq ($(DEBUG), yes)
	CFLAGS += -g
else
	CFLAGS += -O3 -DNDEBUG
endif

all: mrproper $(EXEC)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

