#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define DATA_FILE "data.txt"
#define ARRAY_SIZE 5

void bubbleSort(int numbers[]) {
    for (int pass = 0; pass < ARRAY_SIZE - 1; pass++) {
        for (int index = 0; index < ARRAY_SIZE - pass - 1; index++) {
            if (numbers[index] > numbers[index + 1]) {
                int temp = numbers[index];
                numbers[index] = numbers[index + 1];
                numbers[index + 1] = temp;
            }
        }
    }
}

int main() {
    int originalNumbers[ARRAY_SIZE] = {5, 3, 1, 4, 2};
    FILE *filePointer;

    printf("Before Sorting:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", originalNumbers[i]);
    }
    printf("\n");

    filePointer = fopen(DATA_FILE, "w");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        fprintf(filePointer, "%d ", originalNumbers[i]);
    }
    fclose(filePointer);

    if (fork() == 0) {  
        // Child Process
        int numbersToSort[ARRAY_SIZE];

        filePointer = fopen(DATA_FILE, "r");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            fscanf(filePointer, "%d", &numbersToSort[i]);
        }
        fclose(filePointer);

        bubbleSort(numbersToSort);

        filePointer = fopen(DATA_FILE, "w");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            fprintf(filePointer, "%d ", numbersToSort[i]);
        }
        fclose(filePointer);

        exit(0);
    } 
    else {  
        wait(NULL);

        filePointer = fopen(DATA_FILE, "r");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            fscanf(filePointer, "%d", &originalNumbers[i]);
        }
        fclose(filePointer);

        printf("After Sorting:\n");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", originalNumbers[i]);
        }
        printf("\n");
    }

    return 0;
}
