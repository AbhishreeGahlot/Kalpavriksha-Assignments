#include <stdio.h>
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
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);

    int arr[N] = {9, 7, 5, 3, 1};

    printf("Before Sorting:\n");
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    if (fork() == 0) {
        int data[N];
        read(fd1[0], data, sizeof(data));
        sort(data);
        write(fd2[1], data, sizeof(data));
    } else {
        write(fd1[1], arr, sizeof(arr));
        wait(NULL);
        read(fd2[0], arr, sizeof(arr));

        printf("After Sorting:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }
}
