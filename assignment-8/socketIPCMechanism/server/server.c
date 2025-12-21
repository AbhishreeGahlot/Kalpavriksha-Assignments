#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

pthread_mutex_t lock;

void* handle_client(void* socket_desc) {
    int sock = *(int*)socket_desc;
    int choice, amount;
    FILE *fp;

    read(sock, &choice, sizeof(choice));

    pthread_mutex_lock(&lock);
    fp = fopen("../resource/accountDB.txt", "r+");
    int balance;
    fscanf(fp, "%d", &balance);

    if (choice == 1) {           // Withdraw
        read(sock, &amount, sizeof(amount));
        if (amount <= balance) {
            balance -= amount;
            rewind(fp);
            fprintf(fp, "%d", balance);
            write(sock, &balance, sizeof(balance));
        } else {
            int fail = -1;
            write(sock, &fail, sizeof(fail));
        }
    }
    else if (choice == 2) {      // Deposit
        read(sock, &amount, sizeof(amount));
        balance += amount;
        rewind(fp);
        fprintf(fp, "%d", balance);
        write(sock, &balance, sizeof(balance));
    }
    else if (choice == 3) {      // Display
        write(sock, &balance, sizeof(balance));
    }

    fclose(fp);
    pthread_mutex_unlock(&lock);
    close(sock);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(client);

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    listen(server_fd, 5);

    printf("ATM Server Running...\n");

    while ((client_sock = accept(server_fd, (struct sockaddr*)&client, &c))) {
        pthread_t t;
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_sock;
        pthread_create(&t, NULL, handle_client, new_sock);
    }
}
