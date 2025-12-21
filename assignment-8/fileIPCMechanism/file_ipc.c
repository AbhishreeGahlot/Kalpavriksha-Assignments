#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE_NAME "data.txt"
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
    int arr[N] = {5, 3, 1, 4, 2};

    printf("Before Sorting:\n");
    for (int i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    FILE *fp = fopen(FILE_NAME, "w");
    for (int i = 0; i < N; i++)
        fprintf(fp, "%d ", arr[i]);
    fclose(fp);

    if (fork() == 0) {  // Child
        int data[N];
        fp = fopen(FILE_NAME, "r");
        for (int i = 0; i < N; i++)
            fscanf(fp, "%d", &data[i]);
        fclose(fp);

        sort(data);

        fp = fopen(FILE_NAME, "w");
        for (int i = 0; i < N; i++)
            fprintf(fp, "%d ", data[i]);
        fclose(fp);
        exit(0);
    } else {           // Parent
        wait(NULL);
        fp = fopen(FILE_NAME, "r");
        for (int i = 0; i < N; i++)
            fscanf(fp, "%d", &arr[i]);
        fclose(fp);

        printf("After Sorting:\n");
        for (int i = 0; i < N; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }
}
