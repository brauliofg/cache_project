# Cache Simulator CS 3853 Spring 2021– Group #8
# 1. Objective
The goal of this project is to help understand the internal operations of CPU caches. A Level 1 cache for a 32-bit CPU is simulated in this project. Assume a 32-bit data bus. The cache is command line configurable to be direct-mapped, 2-way, 4-way, 8-way, or 16-way set associative and implement both round-robin and random replacement policies for performance comparisons.

# 2. General Project Descriptions
##  Simulator Input and Memory Trace Files
The simulator uses the following input parameters:
1. –f <trace file name> [ name of text file with the trace ] 
2. –s <cache size in KB> [ 1 KB to 8 MB ] 
3. –b <block size> [ 4 bytes to 64 bytes ] 
4. –a <associativity> [ 1, 2, 4, 8, 16 ] 
5. –r <replacement policy> [ RR or RND or LRU]

Sample command line:
`./sim -f trace1.trc -s 512 -b 16 -a 2 -r RR`

This command line reads the trace file named "trace1.trc", configures a total cache size of 512 KB with block size of 16 bytes/block. It is 2-way set associative and uses a replacement policy of Round Robin. Assume a write-through policy. **Cost of cache memory: $0.09 / KB**

## Simulator Outputs
Simulator outputs the simulation result to the screen (*standard out*, or *stdout*). The output has a short header formatted as follows:
`Cache Simulator CS 3853 Spring 2021 - Group #8`

The parsed command line will be printed with the following format:<br>
**Trace File:**  &lt;name of trace file&gt;<br>
**Cache Size:**  &lt;size typed in KB&gt;<br>
**Block Size:**  &lt;size typed in bytes&gt;<br>
**Associativity:**  &lt;direct, 2-way, 4-way, etc.&gt;<br>
**R-Policy:**  &lt;characters typed in&gt;<br>

### MILESTONE #1: Input Parameters and Calculated Values:
Command Line:
`./sim -f Trace2A.trc -s 512 -b 16 -a 8 -r RND`

Output:
```
Cache Simulator CS 3853 Spring 2021 - Group #8

Trace File: Trace2A.trc

***** Cache Input Parameters *****
Cache Size: 512 KB
Block Size: 16 bytes
Associativity: 8
Replacement Policy: Random

***** Cache Calculated Values *****

Total # Blocks: 32768
Tag Size: 16 bits
Index Size: 12 bits
Total # Rows: 4096
Overhead Size: 69632 bytes
Implementation Memory Size: 580.00 KB (593920 bytes) 
Cost: $40.60
```
### MILESTONE #2: Simulation Results
```
***** CACHE SIMULATION RESULTS *****

Total Cache Accesses: 282168 
Cache Hits: 275489 
Cache Misses: 6679 
--- Compulsory Misses: 6656 
--- Conflict Misses: 23

***** ***** CACHE HIT & MISS RATE: ***** ***** 

Hit Rate: 97.6330% 
Miss Rate: 2.3670% 
CPI: 4.13 Cycles/Instruction 
Unused Cache Space: 462.19 KB / 580.00 KB = 79.69% Waste: $32.35 
Unused Cache Blocks: 26112 / 32768 
```
