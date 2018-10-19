CC = gcc
CFLAGS = -Wall
TARGET = implicit_barrier_openmp
OPENMPFLAGS = -fopenmp
all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(OPENMPFLAGS)
clean:
	$(RM) $(TARGET)
