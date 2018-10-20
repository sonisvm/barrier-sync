CC = gcc
CFLAGS = -Wall
TARGET = counting_barrier_delay
OPENMPFLAGS = -fopenmp
all: $(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(OPENMPFLAGS)
clean:
	$(RM) $(TARGET)
