#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sim.h"

int main(int argc, char *argv[]){
    //Check if number of command line arguments are correct
    if(argc==1 || (argc%2)!=1){
        printf("Usage: ./sim -f <trace file name> -s <cache size in KB> -b <block size> -a <associativity> -r <replacement policy>\n");
        return -1;
    }

    //Verify data and initialize CacheData struct with command line arguments
    CacheData *cache = initCacheData(argc, argv);
    
    //Initializes CalcData struct based on cache struct
    CalcData *calcData = initCalcData(cache);
    
    //Print Cache Input Parameters
    printCacheInput(cache);

    //Free struct that holds information from command line
    freeCache(cache);
    //Free struct that holds calc information
    free(calcData);
    return 0;
}
