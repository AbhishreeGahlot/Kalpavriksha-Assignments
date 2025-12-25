#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define ARRAY_SIZE 5

struct Message {
    long messageType;
    int numbers[ARRAY_SIZE];
};

void bubbleSort(int values[]) {
    for (int pass = 0; pass < ARRAY_SIZE - 1; pass++) {
        for (int index = 0; index < ARRAY_SIZE - pass - 1; index++) {
            if (values[index] > values[index + 1]) {
                int temp = values[index];
                values[index] = values[index + 1];
                values[index + 1] = temp;
            }
        }
    }
}

int main() {
    key_t messageKey = ftok("msg", 65);
    int messageQueueId = msgget(messageKey, 0666 | IPC_CREAT);

    struct Message messageBuffer;
    messageBuffer.messageType = 1;

    int originalNumbers[ARRAY_SIZE] = {6, 2, 9, 1, 4};

    printf("Before Sorting:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        messageBuffer.numbers[i] = originalNumbers[i];
        printf("%d ", originalNumbers[i]);
    }
    printf("\n");

    msgsnd(messageQueueId, &messageBuffer, sizeof(messageBuffer.numbers), 0);

    if (fork() == 0) {
        msgrcv(messageQueueId, &messageBuffer, sizeof(messageBuffer.numbers), 1, 0);
        bubbleSort(messageBuffer.numbers);
        messageBuffer.messageType = 2;
        msgsnd(messageQueueId, &messageBuffer, sizeof(messageBuffer.numbers), 0);
    } else {
        msgrcv(messageQueueId, &messageBuffer, sizeof(messageBuffer.numbers), 2, 0);
        printf("After Sorting:\n");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", messageBuffer.numbers[i]);
        }
        printf("\n");
        msgctl(messageQueueId, IPC_RMID, NULL);
    }

    return 0;
}
