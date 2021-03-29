#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define COST_OF_CACHE 0.09

//CacheData struct that holds cache information
typedef struct CacheData
{
    int cacheSize;
    int blockSize;
    int associativity;
    char *fileName;
    char *replacementPolicy;

}CacheData;

//CalcData struct that holds calculated data for the cache
typedef struct CalcData
{
    int totalBlocks;
    int offset;
    int tagSize;
    int indexSize;
    int totalRows;
    int overHeadSize;
    int implementationBytesMemSize;
    double cost;
    
}CalcData;

//Frees CacheData struct
void freeCache(CacheData *cache){
    free(cache->fileName);
    free(cache->replacementPolicy);
    free(cache);
}

/* Initializes CacheData struct with command line arguments and exits program
   if unknown flag is found */
CacheData *initCacheData(int argc, char* argv[]){
    CacheData *cache;
    cache = (CacheData*)malloc(sizeof(CacheData));
    cache->fileName = (char*)malloc(sizeof(char)*50);
    cache->replacementPolicy = (char*)malloc(sizeof(char)*4);

    int i;
    for(i=1;i<argc;i+=2){
        if(strcmp(argv[i], "-f")==0){
            strcpy(cache->fileName, argv[i+1]);
        }
        else if(strcmp(argv[i], "-s")==0){
            cache->cacheSize = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-b")==0){
            cache->blockSize = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-a")==0){
            cache->associativity = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-r")==0){
            strcpy(cache->replacementPolicy, argv[i+1]);
        }
        else{
            printf("Unknown input.\nExiting.\n");
            freeCache(cache);
            exit(0);
        }
    }

    return cache;
}

// Calulates cache values and stores them in CalcData struct using CacheData struct. Uses change of base formula for log base 2 on some calculations.
CalcData *initCalcValues(CacheData *cacheData){	
    //Constants for calculations
    const int DATA_BUS = 32;
    const int ONE_KB = 1024;
    const double DOUBLE_TWO = (double)2;

    CalcData *calcData = (calcData*)malloc(sizeof(CalcData));

    //Calculate offset uses change base formula
    calcData->offset = (int)(log10((double)cacheData->blockSize) / log10(DOUBLE_TWO))

    //Calculate total blocks
    calcData->totalBlocks = cacheData->cacheSize * ONE_KB / cacheData->blockSize;

    //Calculate index size uses change base formula
    calcData->indexSize = (int)(log10((double)(cacheData->cacheSize * ONE_KB / (cacheData->associativity * cacheData->blockSize))) / log10(DOUBLE_TWO));

    //Calculate tag size
    calcData->tagSize = DATA_BUS - offset - calcData->indexSize;

    //Calculate total rows
    calcData->totalRows = (int)pow(DOUBLE_TWO, (double)calcData->indexSize);

    //Calculate overhead size
    calcData->overHeadSize = calcData->totalRows + (int)pow(DOUBLE_TWO, (double)calcData->tagSize);

    //Calculate total implementation memory size in bytes
    calcData->implementationBytesMemSize = cacheData->cacheSize * ONE_KB + overHeadSize;

    //Calculate total cost
    calcData->cost = calcData->implementationBytesMemSize / ONE_KB * COST_OF_CACHE;

    return calcData;

}

//Prints Cache Input Parameters
void printCacheInput(CacheData *cache){
    printf("Cache Simulator CS 3853 Spring 2021 - Group #8\n\n");
    printf("Trace File: %s\n", cache->fileName);
    printf("\n***** Cache Input Parameters *****\n\n");
    printf("Cache Size:             %d KB\n", cache->cacheSize);
    printf("Block Size:             %d bytes\n", cache->blockSize);
    printf("Associativity:          %d\n", cache->associativity);
    if(strcmp(cache->replacementPolicy, "RR")==0){
        printf("Replacement Policy:     Round-robin\n");
    }
    else if(strcmp(cache->replacementPolicy, "RND")==0){
        printf("Replacement Policy:     Random\n");
    }
    else if(strcmp(cache->replacementPolicy, "LRU")==0){
        printf("Replacement Policy:     Least Recently Used\n");
    }
}
