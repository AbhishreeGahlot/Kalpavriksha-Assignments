#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define N 5

struct msg {
    long type;
    int data[N];
};

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
    key_t key = ftok("msg", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct msg m;
    m.type = 1;
    int arr[N] = {6, 2, 9, 1, 4};

    printf("Before Sorting:\n");
    for (int i = 0; i < N; i++) {
        m.data[i] = arr[i];
        printf("%d ", arr[i]);
    }
    printf("\n");

    msgsnd(msgid, &m, sizeof(m.data), 0);

    if (fork() == 0) {
        msgrcv(msgid, &m, sizeof(m.data), 1, 0);
        sort(m.data);
        m.type = 2;
        msgsnd(msgid, &m, sizeof(m.data), 0);
    } else {
        msgrcv(msgid, &m, sizeof(m.data), 2, 0);
        printf("After Sorting:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", m.data[i]);
        printf("\n");
        msgctl(msgid, IPC_RMID, NULL);
    }
}
