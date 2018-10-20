MPICC = mpicc
CFLAGS = -Wall
LFLAGS = -lm
OPENMPFLAGS = -fopenmp
TARGET = tournament_counting_barrier
all: $(TARGET)
$(TARGET): $(TARGET).c
	$(MPICC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS) $(OPENMPFLAGS)
clean:
	$(RM) $(TARGET)
