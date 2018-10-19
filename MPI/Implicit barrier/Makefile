MPICC = mpicc
CFLAGS = -Wall
LFLAGS = -lm
TARGET = implicit_barrier_mpi
all: $(TARGET)
$(TARGET): $(TARGET).c
	$(MPICC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LFLAGS)
clean:
	$(RM) $(TARGET)
