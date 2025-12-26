#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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
    int parentToChildPipe[2];
    int childToParentPipe[2];

    pipe(parentToChildPipe);
    pipe(childToParentPipe);

    int originalNumbers[ARRAY_SIZE] = {9, 7, 5, 3, 1};

    printf("Before Sorting:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", originalNumbers[i]);
    }
    printf("\n");

    if (fork() == 0) {
        int receivedNumbers[ARRAY_SIZE];
        read(parentToChildPipe[0], receivedNumbers, sizeof(receivedNumbers));
        bubbleSort(receivedNumbers);
        write(childToParentPipe[1], receivedNumbers, sizeof(receivedNumbers));
    } else {
        write(parentToChildPipe[1], originalNumbers, sizeof(originalNumbers));
        wait(NULL);
        read(childToParentPipe[0], originalNumbers, sizeof(originalNumbers));

        printf("After Sorting:\n");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", originalNumbers[i]);
        }
        printf("\n");
    }

    return 0;
}
