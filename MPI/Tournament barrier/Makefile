MPICC = mpicc
CFLAGS = -Wall
LFLAGS = -lm
TARGET = tournament_barrier
all: $(TARGET)
$(TARGET): $(TARGET).c
	$(MPICC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS)
clean:
	$(RM) $(TARGET)
