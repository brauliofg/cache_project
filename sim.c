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
    CacheData *cacheData = initCacheData(argc, argv);
    
    //Initializes CalcData struct based on cache struct
    CalcData *calcData = initCalcData(cacheData);
    cacheStruct **cache;
    cache = (cacheStruct**)malloc(sizeof(cacheStruct)*calcData->totalRows);
    int i, j;
    for(i=0;i<calcData->totalRows;i++){
        cache[i] = malloc(sizeof(cacheStruct)*cacheData->associativity);
    }
    for(i=0; i<calcData->totalRows; i++){
        for(j=0; j<cacheData->associativity; j++){
            cache[i][j].valid = 0;
        }
    }
    
    //Print Cache Input Parameters
    printCacheInput(cacheData);
    
    //Print Cache Calculated Results
    printCacheResults(calcData);
    
    //Parse file
    parseFile(cacheData, calcData, cache);
    
    //Print Cache Simulation Results
    printCacheSimResults();
    
    //Frees all malloced data
    freeCache(cache, calcData);
    freeCacheData(cacheData);
    free(calcData);
    return 0;
}
