#include <stdlib.h>
#include <stdio.h>

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