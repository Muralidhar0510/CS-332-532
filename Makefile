

CC = gcc
CFLAGS = -Wall -pthread
SRC = mchapala_HW04.c
OBJ = $(SRC:.c=.o)
EXEC = my_program
all: $(EXEC)
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ) $(EXEC)
run: $(EXEC)
	./$(EXEC)

