#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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
    key_t sharedMemoryKey = ftok("shm", 65);
    int sharedMemoryId = shmget(sharedMemoryKey, ARRAY_SIZE * sizeof(int), 0666 | IPC_CREAT);
    int *sharedNumbers = (int *)shmat(sharedMemoryId, NULL, 0);

    int initialNumbers[ARRAY_SIZE] = {8, 3, 6, 1, 2};
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sharedNumbers[i] = initialNumbers[i];
    }

    printf("Before Sorting:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", sharedNumbers[i]);
    }
    printf("\n");

    if (fork() == 0) {
        bubbleSort(sharedNumbers);
    } else {
        wait(NULL);
        printf("After Sorting:\n");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", sharedNumbers[i]);
        }
        printf("\n");
        shmctl(sharedMemoryId, IPC_RMID, NULL);
    }

    return 0;
}
