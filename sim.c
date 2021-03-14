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

    //Get information from command line
    int i;
    InputData *input = makeInputStruct();
    for(i=1;i<argc;i+=2){
        if(strcmp(argv[i], "-f")==0){
            strcpy(input->fileName, argv[i+1]);
        }
        else if(strcmp(argv[i], "-s")==0){
            input->cacheSize = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-b")==0){
            input->blockSize = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-a")==0){
            input->associativity = atoi(argv[i+1]);
        }
        else if(strcmp(argv[i], "-r")==0){
            strcpy(input->replacementPolicy, argv[i+1]);
        }
        else{
            printf("Unknown input.\nExiting.\n");
            freeInput(input);
            return -1;
        }
    }
    printInput(input);

    //Free struct that holds information from command line
    freeInput(input);
    return 0;
}