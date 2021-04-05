#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
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
            printf("Unknown input: %s\nExiting.\n",argv[i]);
            freeCache(cache);
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
void printCacheInput(CacheData *cache){
    printf("Cache Simulator CS 3853 Spring 2021 - Group #08\n\n");
    printf("Trace File: %s\n", cache->fileName);
    printf("\n***** Cache Input Parameters *****\n\n");
    printf("Cache Size:		    	%d KB\n", cache->cacheSize);
    printf("Block Size:             	%d bytes\n", cache->blockSize);
    printf("Associativity:          	%d\n", cache->associativity);
    if(strcmp(cache->replacementPolicy, "RR")==0){
        printf("Replacement Policy:		Round-robin\n");
    }
    else if(strcmp(cache->replacementPolicy, "RND")==0){
        printf("Replacement Policy:		Random\n");
    }
    else if(strcmp(cache->replacementPolicy, "LRU")==0){
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


void parseAndPrintFile(char *fileName)
{
    FILE* file = fopen(fileName, "r"); 
    char line[256];
    char eipLeng[20] = "-1";
    char eipAddress[20] = "-1"; 
    char dstMAddress[20] = "-1";
    char srcMAddress[20] = "-1";
    char * token;
    int lineCount = 0;
    
    // token flag
    // 0 = no needed value identified
    // 1 = EIP Length
    // 2 = EIP Address
    // 3 = dstM Address
    // 4 = srcM Address
    int tokenFlag = 0; 
    
    printf("\nPrinting first 20 addresses for milestone #1:\n");
    while (fgets(line, sizeof(line), file)) 
    {
        //printf("\n%s",line);

        token = strtok(line, " ");
        while( token != NULL ) 
        {
            
            // if a needed value was identified store this current token in the appropriate variable
            // 1 = EIP Length
            // 2 = EIP Address
            // 3 = dstM Address
            // 4 = srcM Address
            if(tokenFlag == 1) 
            {
                strncpy(eipLeng, token+1, 2);
                tokenFlag = 2;
            }
         
            else if(tokenFlag == 2) 
            {
                strcpy(eipAddress, token);
                tokenFlag = 0;
            } 
            else if(tokenFlag == 3) 
            {
                strcpy(dstMAddress, token);
                tokenFlag = 0;
            } 
            else if(tokenFlag == 4) 
            {
                strcpy(srcMAddress, token);
                tokenFlag = 0;
            }
            
            
            if (strcmp(token,"EIP") == 0)
            {
                tokenFlag = 1;
            }
            else if(strcmp(token,"dstM:") == 0) 
            {
                tokenFlag = 3;
            }
            else if (strcmp(token,"srcM:") == 0) 
            {
                tokenFlag = 4;
            }
            
            token = strtok(NULL, " ");

        }
        
        if((strcmp(eipLeng,"-1") != 0) && (strcmp(eipAddress,"-1") != 0) && (strcmp(dstMAddress,"-1") != 0) && (strcmp(srcMAddress,"-1") != 0))
        {
            // printf("==============================================================");
            // printf("\nPrinting EIP Length: '%s'", eipLeng); //debug
            // printf("\nPrinting EIP Address: '%s'", eipAddress); //debug
            // printf("\nPrinting dstM Address: '%s'", dstMAddress); //debug
            // printf("\nPrinting srcM Address: '%s'", srcMAddress); //debug
            // printf("\n==============================================================\n\n\n");
            if(lineCount < 20)
            {
                printf("0x%x: (%d)\n", (int)strtol(eipAddress, NULL, 16), (int)strtol(eipLeng, NULL, 16)); //debug
                lineCount++;
            }
            if((strcmp(dstMAddress,"00000000") != 0) && (lineCount < 20))
            {
                printf("0x%x: (%d)\n", (int)strtol(dstMAddress, NULL, 16),4); //debug
                lineCount++;
            }
            if((strcmp(srcMAddress,"00000000") != 0) && (lineCount < 20))
            {
                printf("0x%x: (%d)\n", (int)strtol(srcMAddress, NULL, 16),4); //debug    
                lineCount++;
            }
            
            strcpy(eipLeng, "-1");
            strcpy(eipAddress, "-1");
            strcpy(dstMAddress, "-1");
            strcpy(srcMAddress, "-1");
        }
        
    }

    fclose(file);
}
