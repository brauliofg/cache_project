#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define COST_OF_CACHE 0.09
int totalAddresses=0;
int totalCacheAccess=0;
int cacheHits=0;
int cacheMisses=0;
int compMisses=0;
int confMisses=0;
int cycles=0;
int totalInstruction=0;

//CacheData struct that holds cache information
typedef struct CacheData{
    int cacheSize;
    int blockSize;
    int associativity;
    char *fileName;
    char *replacementPolicy;

}CacheData;

typedef struct cacheStruct {
    int valid;
    int clock;
    unsigned int tag;
} cacheStruct;

//CalcData struct that holds calculated data for the cache
typedef struct CalcData{
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
void freeCacheData(CacheData *cacheData){
    free(cacheData->fileName);
    free(cacheData->replacementPolicy);
    free(cacheData);
}

void freeCache(cacheStruct **cache, CalcData *calcData){
    int i;
    for(i=0;i<calcData->totalRows;i++){
        free(cache[i]);
    }
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
            printf("Unknown input: %s\nExiting.\n",argv[i]);
            freeCacheData(cache);
            exit(0);
        }
    }

    return cache;
}

// Calulates cache values and stores them in CalcData struct using CacheData struct. Uses change of base formula for log base 2 on some calculations.
CalcData *initCalcData(CacheData *cacheData){	
    //Constants for calculations
    const int DATA_BUS = 32;
    const int ONE_KB = 1024;
    const double DOUBLE_TWO = (double)2;

    CalcData *calcData = (CalcData*)malloc(sizeof(CalcData));

    //Calculate offset uses change base formula
    calcData->offset = (int)(log10((double)cacheData->blockSize) / log10(DOUBLE_TWO));

    //Calculate total blocks
    calcData->totalBlocks = cacheData->cacheSize * ONE_KB / cacheData->blockSize;

    //Calculate index size uses change base formula
    calcData->indexSize = (int)(log10((double)(cacheData->cacheSize * ONE_KB / (cacheData->associativity * cacheData->blockSize))) / log10(DOUBLE_TWO));

    //Calculate tag size
    calcData->tagSize = DATA_BUS - calcData->offset - calcData->indexSize;

    //Calculate total rows
    calcData->totalRows = (int)pow(DOUBLE_TWO, (double)calcData->indexSize);

    //Calculate overhead size
    calcData->overHeadSize = calcData->totalRows + (int)pow(DOUBLE_TWO, (double)calcData->tagSize);

    //Calculate total implementation memory size in bytes
    calcData->implementationBytesMemSize = cacheData->cacheSize * ONE_KB + calcData->overHeadSize;

    //Calculate total cost
    calcData->cost = calcData->implementationBytesMemSize / ONE_KB * COST_OF_CACHE;

    return calcData;
}

//Prints Cache Input Parameters
void printCacheInput(CacheData *cacheData){
    printf("Cache Simulator CS 3853 Spring 2021 - Group #08\n\n");
    printf("Trace File: %s\n", cacheData->fileName);
    printf("\n***** Cache Input Parameters *****\n\n");
    printf("Cache Size:		    	%d KB\n", cacheData->cacheSize);
    printf("Block Size:             	%d bytes\n", cacheData->blockSize);
    printf("Associativity:          	%d\n", cacheData->associativity);
    if(strcmp(cacheData->replacementPolicy, "RR")==0){
        printf("Replacement Policy:		Round-robin\n");
    }
    else if(strcmp(cacheData->replacementPolicy, "RND")==0){
        printf("Replacement Policy:		Random\n");
    }
    else if(strcmp(cacheData->replacementPolicy, "LRU")==0){
        printf("Replacement Policy:		Least Recently Used\n");
    }
}

//Prints Cache Caclculated Results
void printCacheResults(CalcData *calcData){
    double implementationInKB = (double)calcData->implementationBytesMemSize / 1024;
    printf("\n***** Cache Calculated Values *****\n\n");
    printf("Total # Blocks:			%d\n", calcData->totalBlocks);
    printf("Tag Size:			%d bits\n", calcData->tagSize);
    printf("Index Size:			%d bits\n", calcData->indexSize);
    printf("Total # Rows:			%d\n", calcData->totalRows);
    printf("Overhead Size:			%d bytes\n", calcData->overHeadSize);
    printf("Implementation Memory Size:	%0.2lf KB (%d bytes)\n", implementationInKB, calcData->implementationBytesMemSize);
    printf("Cost:				$%0.2lf\n", calcData->cost);
}

//Prints Cache Simulation Results
void printCacheSimResults(){
    printf("\n***** Cache Simulation Results *****\n\n");
    printf("Total Cache Accesses:		%d (%d addresses)\n", totalCacheAccess, totalAddresses);
    printf("Cache Hits:			%d\n", cacheHits);
    printf("Cache Misses:			%d\n", cacheMisses);
    printf("--- Compulsory Misses:		%d\n", compMisses);
    printf("--- Conflict Misses:		%d\n", confMisses);
}

//Prints Cache Hit and Miss Rate
void printCacheHitAndMissRate(CacheData *cacheData, CalcData *calcData){
    //Calculates hit rate
    double hitRate = (double)cacheHits/(double)totalCacheAccess;
    //Calculates unused cache space KB
    double unusedCacheSpace = (((double)calcData->totalBlocks - (double)compMisses) * ((double)cacheData->blockSize + (1.0 / 8.0) + ((double)calcData->tagSize / 8.0))) / 1024.0;
    //Calculates total memory for cache in KB
    double totalMemoryKB = (double)calcData->implementationBytesMemSize / 1024.0;

    printf("\n***** ***** CACHE HIT & MISS RATE: ***** *****\n\n");
    
    printf("Hit Rate:			%.4f%%\n", hitRate * 100.0);
    printf("Miss Rate:			%.4f%%\n", (1.0 - hitRate) * 100.0);
    printf("CPI:				%0.2lf Cycles/Instruction (%d)\n", (double)cycles/totalInstruction, totalInstruction);
    printf("Unused Cache Space:		%.2f KB / %.2f KB = %.2f%%   Waste: $%.2f\n", unusedCacheSpace, totalMemoryKB, 
    (unusedCacheSpace / totalMemoryKB) * 100.0, unusedCacheSpace * COST_OF_CACHE);
    printf("Unused Cache Blocks:		%d / %d\n", calcData->totalBlocks - compMisses, calcData->totalBlocks);
}

//Implements round robin replacement policy
void cacheAddRR(CacheData *cacheData, CalcData *calcData, cacheStruct **cache, unsigned int iOffset, unsigned int iIndex, unsigned int iTag, int iLength, int *arr){
    int minClock=-1;
    totalCacheAccess++;
    
    int j;
    for(j=0; j<cacheData->associativity; j++){
        if(cache[iIndex][j].valid==1 && cache[iIndex][j].tag == iTag){
            cacheHits++;
            cycles++;
            break;
        }
        else if(cache[iIndex][j].valid == 0){
            cache[iIndex][j].valid=1;
            cache[iIndex][j].tag = iTag;
            cycles+=(4 * ((int)ceil(cacheData->blockSize/4.0)));
            cacheMisses++;
            compMisses++;
            break;
        }
        else if(j==(cacheData->associativity)-1){
            cacheMisses++;
            confMisses++;
            cache[iIndex][arr[iIndex]].tag=iTag;
            if(arr[iIndex]+1==cacheData->associativity){
                arr[iIndex]=0;
            }
            else{
                arr[iIndex]=arr[iIndex]+1;
            }
            cycles+=(4 * ((int)ceil(cacheData->blockSize/4.0)));
        }
    }
    
    unsigned int offsetMax = log(cacheData->blockSize)/log(2); //Gets number of bits offset needs        
    offsetMax = (int)(pow(2, offsetMax)-1); //Gets limit offset that can be accessed without needing a new cache access
    int lastBit = iOffset + iLength - 1; //Finds value of last bit to be accessed
    //Checks to see if last bit is out of bounds, sets iIndex to next valid index, calls cacheAddRR again to access the rest
    if(lastBit > offsetMax){
        if((iIndex+1) < calcData->totalRows) {
            iIndex+=1;
        }
        else
            iIndex=0;
        cacheAddRR(cacheData, calcData, cache, 0, iIndex, iTag, iLength-(offsetMax-iOffset+1), arr);
    }
}

//Address is deconstructed into tag, index, and offset and passed into appropriate function based on replacement policy chosen
void accessAddress(CacheData *cacheData, CalcData *calcData, cacheStruct **cache, char addressToAdd[20], char eipLeng[20], int *arr){
    unsigned int iAddress = (int)strtol(addressToAdd, NULL, 16); //Converts address into integer
    unsigned int iTag, iOffset, iIndex;
    int iLength = (int)strtol(eipLeng, NULL, 10); //Converts instruction length into integer
    int bitOffset;
    
    bitOffset = log(cacheData->blockSize)/log(2); //Gets number of bits offset needs        
    iOffset = (int)(pow(2, bitOffset)-1) & iAddress; //Gets offset's integer value
    
    iAddress = iAddress>>bitOffset; //Removes offset bits from address
    iIndex = (int)(pow(2, calcData->indexSize)-1) & iAddress; //Gets index's integer value
    
    iTag = iAddress>>calcData->indexSize; //Sets iTag by removing index bits
    
    if(strcmp(cacheData->replacementPolicy, "RR")==0){
        cacheAddRR(cacheData, calcData, cache, iOffset, iIndex, iTag, iLength, arr);
    }
}

//Parses input file and passes addresses into cache
void parseFile(CacheData *cacheData, CalcData *calcData, cacheStruct **cache){
    FILE* file = fopen(cacheData->fileName, "r"); 
    char line[256];
    char eipLeng[20] = "-1";
    char eipAddress[20] = "-1"; 
    char dstMAddress[20] = "-1";
    char srcMAddress[20] = "-1";
    char * token;
    int i, arr[calcData->totalRows];
    for(i=0;i<calcData->totalRows;i++){
        arr[i]=0;
    }
    
    // token flag
    // 0 = no needed value identified
    // 1 = EIP Length
    // 2 = EIP Address
    // 3 = dstM Address
    // 4 = srcM Address
    int tokenFlag = 0; 
    
    while (fgets(line, sizeof(line), file)){
        token = strtok(line, " ");
        while( token != NULL ){
            // if a needed value was identified store this current token in the appropriate variable
            // 1 = EIP Length
            // 2 = EIP Address
            // 3 = dstM Address
            // 4 = srcM Address
            if(tokenFlag == 1){
                strncpy(eipLeng, token+1, 2);
                tokenFlag = 2;
            }
            else if(tokenFlag == 2) {
                strcpy(eipAddress, token);
                tokenFlag = 0;
            } 
            else if(tokenFlag == 3) {
                strcpy(dstMAddress, token);
                tokenFlag = 0;
            } 
            else if(tokenFlag == 4) {
                strcpy(srcMAddress, token);
                tokenFlag = 0;
            }
            
            if (strcmp(token,"EIP") == 0){
                tokenFlag = 1;
            }
            else if(strcmp(token,"dstM:") == 0){
                tokenFlag = 3;
            }
            else if (strcmp(token,"srcM:") == 0) {
                tokenFlag = 4;
            }
            token = strtok(NULL, " ");
        }
        
        if((strcmp(eipLeng,"-1") != 0) && (strcmp(eipAddress,"-1") != 0) && (strcmp(dstMAddress,"-1") != 0) && (strcmp(srcMAddress,"-1") != 0)){
            accessAddress(cacheData, calcData, cache, eipAddress, eipLeng, arr);
            totalAddresses++;
            totalInstruction++;
            cycles+=2;
            if((strcmp(dstMAddress,"00000000") != 0)){
                accessAddress(cacheData, calcData, cache, dstMAddress, "4", arr);
                totalAddresses++;
                cycles++;
            }
            if((strcmp(srcMAddress,"00000000") != 0)){
                accessAddress(cacheData, calcData, cache, srcMAddress, "4", arr);
                totalAddresses++;
                cycles++;
            }
            
            strcpy(eipLeng, "-1");
            strcpy(eipAddress, "-1");
            strcpy(dstMAddress, "-1");
            strcpy(srcMAddress, "-1");
        }
        
    }

    fclose(file);
}
