OMPFLAGS = -fopenmp
LMFLAG = -lm

CC = gcc
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

all: OMP_default.out OMP_sensereversal.out

OMP_default.out: OMP_default.c
	$(CC) -o $@ $(LDFLAGS) $^ $(OMPFLAGS)

OMP_sensereversal.out: OMP_sensereversal.c
	$(CC) -o $@ $(LDFLAGS) $^ $(OMPFLAGS)

clean:
	rm -f OMP_sensereversal.out OMP_default.out