#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quicksort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
	   int (*comp)(void *, void *));

// Helper functions
int compareNumbers(void* strN1, void* strN2) {
		int n1 = atoi(strN1);
		int n2 = atoi(strN2);
		if(n1 < n2) {
			return -1;
		} else if(n1 > n2) {
			return 1;
		} else {
			return 0;
		}
}

int main(int argc, char **argv)
{
	char* inputFileStr;
	char* outputFileStr;
	int sortingNumbers = 0;
	char sortAlgorithm;
	int i = 1;

	// --------- Validation ---------- 
    if(argc < 5){
		printf("Expected at least 5 arguments, (-n, inputFileName.txt, -quicksort / -mergesort, -o outputFileName)\n");
		return -1;
	}
	
	if (strcmp(argv[i], "-n") == 0){
		sortingNumbers = 1;
		i++;
	}

	char* inputFileExtension = argv[i] + strlen(argv[i]) - 4;
	
	if (strcmp(inputFileExtension, ".txt") == 0){
		inputFileStr = argv[i];
		i++;
	}else{
		printf("Invalid input file name, (filename.txt)\n");
		return -1;
	}

	if(strcmp(argv[i], "-quicksort") == 0){
		sortAlgorithm='q';
		i++;
	}else if(strcmp(argv[i], "-mergesort") == 0){
		sortAlgorithm='m';
		i++;
	}else{
		printf("Invalid sorting algorithm, please input -quicksort or -mergesort\n");
		return -1;
	}

	if (strcmp(argv[i], "-o") == 0){
		i++;
		char* outputFileExtension = argv[i] + strlen(argv[i]) - 4;
		if (strcmp(outputFileExtension, ".txt") == 0){
			outputFileStr = argv[i];
		}else{
			printf("Invalid output file name, (filename.txt)\n");
			return -1;
		}
	}else {
		printf("Expected -o , (-n, inputFileName.txt, -quicksort / -mergesort, -o outputFileName)\n");
	}

	printf("%s, %s, %d, %c\n", inputFileStr, outputFileStr, sortingNumbers, sortAlgorithm);
	
	// ---------- File read -----------


	FILE *inputFile;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	inputFile = fopen(inputFileStr, "r");
	if (inputFile == NULL){
        printf("Error opening inputFile\n");
        return -1;
    }

	int inputFileLength = 0;
	while((read = getline(&line, &len, inputFile)) != -1) {
		inputFileLength++;
	}

	fseek(inputFile, 0, SEEK_SET);

	char **inputLines = (char **)malloc(inputFileLength * sizeof(char*));

	int j = 0;
	while((read = getline(&inputLines[j], &len, inputFile)) != -1) {
		j++;
	}

    fclose(inputFile);

	if(sortAlgorithm == 'q'){
		printf("... sorting %s file with quicksort\n", inputFileStr);
		if (sortingNumbers){
			quicksort((void** )inputLines, 0, inputFileLength - 1, (int(*)(void*, void*))(compareNumbers));
		}else{
			quicksort((void** )inputLines, 0, inputFileLength - 1, (int(*)(void*, void*))(strcmp));
		}
	}else{
		printf("... sorting %s file with mergesort\n", inputFileStr);
		if (sortingNumbers){
			mergesort((void** )inputLines, 0, inputFileLength - 1, (int(*)(void*, void*))(compareNumbers));
		}else{
			mergesort((void** )inputLines, 0, inputFileLength - 1, (int(*)(void*, void*))(strcmp));
		}
	}


	FILE *outputFile = fopen(outputFileStr, "wb");

	for(int k = 0; k < inputFileLength; k++){
		fputs(strcat(inputLines[k],""), outputFile);
	}

	fclose(outputFile);
	printf("... Results file can be found at %s\n", outputFileStr);

	return 0;
}
