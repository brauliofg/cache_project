#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct InputData
{
    int cacheSize;
    int blockSize;
    int associativity;
    char *fileName;
    char *replacementPolicy;

}InputData;

InputData *makeInputStruct(){
    InputData *input;
    input = (InputData*)malloc(sizeof(InputData));
    input->fileName = (char*)malloc(sizeof(char)*50);
    input->replacementPolicy = (char*)malloc(sizeof(char)*4);

    return input;
}

void freeInput(InputData *input){
    free(input->fileName);
    free(input->replacementPolicy);
    free(input);
}

void printInput(InputData *input){
    printf("Cache Simulator CS 3853 Spring 2021 - Group #8\n\n");
    printf("Trace File: %s\n", input->fileName);
    printf("\n***** Cache Input Parameters *****\n\n");
    printf("Cache Size:             %d KB\n", input->cacheSize);
    printf("Block Size:             %d bytes\n", input->blockSize);
    printf("Associativity:          %d\n", input->associativity);
    if(strcmp(input->replacementPolicy, "RR")==0){
        printf("Replacement Policy:     Round-robin\n");
    }
    else if(strcmp(input->replacementPolicy, "RND")==0){
        printf("Replacement Policy:     Random\n");
    }
    else if(strcmp(input->replacementPolicy, "LRU")==0){
        printf("Replacement Policy:     Least Recently Used\n");
    }
}