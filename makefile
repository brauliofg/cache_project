all: sim

clean:
	rm -f *.o sim 

sim: sim.c sim.h
	gcc sim.c sim.h -o sim -lm

 
run1:
	./sim -f Traces4Debugging/Trace1.trc -s 512 -b 16 -a 8 -r RND
run2:
	./sim -f Traces4Debugging/Trace1.trc -s 128 -b 8 -a 4 -r RR
run3:
	./sim -f Traces4Debugging/Trace1.trc -s 1024 -b 64 -a 1 -r LRU
