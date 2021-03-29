CC = gcc
CFLAGS = -Wall -g

all: sim

clean:
	rm -f *.o sim 

sim: sim.c sim.h
	gcc sim.c sim.h -o sim -lm

 
run1:
	./sim -f Traces4Debugging/Trace1.trc -s 512 -b 16 -a 8 -r RND