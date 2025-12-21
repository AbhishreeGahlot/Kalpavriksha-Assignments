#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define N 5

void sort(int a[]) {
    for (int i = 0; i < N-1; i++)
        for (int j = 0; j < N-i-1; j++)
            if (a[j] > a[j+1]) {
                int t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
}

int main() {
    key_t key = ftok("shm", 65);
    int shmid = shmget(key, N * sizeof(int), 0666 | IPC_CREAT);
    int *arr = (int *)shmat(shmid, NULL, 0);

    int temp[N] = {8, 3, 6, 1, 2};
    for (int i = 0; i < N; i++)
        arr[i] = temp[i];

    printf("Before Sorting:\n");
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    if (fork() == 0) {
        sort(arr);
    } else {
        wait(NULL);
        printf("After Sorting:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", arr[i]);
        printf("\n");
        shmctl(shmid, IPC_RMID, NULL);
    }
}
