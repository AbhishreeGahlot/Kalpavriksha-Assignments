#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

pthread_mutex_t accountMutex;

void* handleClientRequest(void* socketPointer) {
    int clientSocket = *(int*)socketPointer;
    int userChoice;
    int transactionAmount;
    int accountBalance;

    FILE *accountFile;

    read(clientSocket, &userChoice, sizeof(userChoice));

    pthread_mutex_lock(&accountMutex);

    accountFile = fopen("../resource/accountDB.txt", "r+");
    fscanf(accountFile, "%d", &accountBalance);

    if (userChoice == 1) {
        read(clientSocket, &transactionAmount, sizeof(transactionAmount));
        if (transactionAmount <= accountBalance) {
            accountBalance -= transactionAmount;
            rewind(accountFile);
            fprintf(accountFile, "%d", accountBalance);
            write(clientSocket, &accountBalance, sizeof(accountBalance));
        } else {
            int errorCode = -1;
            write(clientSocket, &errorCode, sizeof(errorCode));
        }
    } 
    else if (userChoice == 2) {
        read(clientSocket, &transactionAmount, sizeof(transactionAmount));
        accountBalance += transactionAmount;
        rewind(accountFile);
        fprintf(accountFile, "%d", accountBalance);
        write(clientSocket, &accountBalance, sizeof(accountBalance));
    } 
    else if (userChoice == 3) {
        write(clientSocket, &accountBalance, sizeof(accountBalance));
    }

    fclose(accountFile);
    pthread_mutex_unlock(&accountMutex);

    close(clientSocket);
    free(socketPointer);
    return NULL;
}

int main() {
    int serverSocket;
    int clientSocket;

    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

    socklen_t clientAddressLength = sizeof(clientAddress);

    pthread_mutex_init(&accountMutex, NULL);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    printf("ATM Server Running...\n");

    while ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength))) {
        pthread_t clientThread;
        int *clientSocketPointer = malloc(sizeof(int));
        *clientSocketPointer = clientSocket;
        pthread_create(&clientThread, NULL, handleClientRequest, clientSocketPointer);
    }

    return 0;
}
