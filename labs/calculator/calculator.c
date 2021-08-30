#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values) {
    int result = 0;
    if (operator == 0){
        for(int i = 0; i < nValues; i++){
            result = result + values[i];
        }

    } else if(operator == 1){
        for(int i = 0; i < nValues; i++){
            result = result - values[i];
        }

    } else if(operator == 2){
        result = 1;
        for(int i = 0; i < nValues; i++){
            result = result * values[i];
        }
    }
    
    printf("result: %d\n", result);
    return result;
}

int* convertStrArrayToInt(int sizeOfArray, char *strValues[]){

    int *intValues = (int*)malloc((sizeOfArray - 2)*sizeof(int));

    for(int i = 2; i < sizeOfArray; i++){
        if(atoi(strValues[i])){
            intValues[i-2] = atoi(strValues[i]);

        }else{
            printf("Expected integer, but received: %s\nTerminating program...\n", strValues[i]);
            exit(-1);
        }
    }
    return intValues;
}

int main(int argc, char *argv[]) {

    if (argc < 3){
        printf("expected atleast 2 arguments (operator, digits)");
        return -1;
    }else{
        if(strcmp(argv[1], "add") == 0){
            // printf("its addition\n");
            calc(0, argc-2, convertStrArrayToInt(argc, argv));

        }else if(strcmp(argv[1], "sub") == 0){
            // printf("its substraction\n");
            calc(1, argc-2, convertStrArrayToInt(argc, argv));

        }else if(strcmp(argv[1], "mult") == 0){
            // printf("its multiplication\n");
            calc(2, argc-2, convertStrArrayToInt(argc, argv));
            
        }else{
            printf("operator not recognized");
            return -1;
        }
    }
}
