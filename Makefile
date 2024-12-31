# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Object files
OBJ = main.o route-records.o

# Executable target
airline_records: $(OBJ)
	$(CC) $(CFLAGS) -o airline_records $(OBJ)

# Compile main.c into main.o
main.o: main.c route-records.h
	$(CC) $(CFLAGS) -c main.c

# Compile route-records.c into route-records.o
route-records.o: route-records.c route-records.h
	$(CC) $(CFLAGS) -c route-records.c

# Clean up object files and executable
clean:
	rm -f *.o airline_records
