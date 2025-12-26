#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    int userChoice;
    int transactionAmount;
    int serverResponse;

    while (1) {
        printf("\n1. Withdraw\n2. Deposit\n3. Display Balance\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        write(clientSocket, &userChoice, sizeof(userChoice));

        if (userChoice == 4) {
            printf("Exiting...\n");
            break;
        }

        if (userChoice == 1 || userChoice == 2) {
            printf("Enter amount: ");
            scanf("%d", &transactionAmount);
            write(clientSocket, &transactionAmount, sizeof(transactionAmount));
        }

        read(clientSocket, &serverResponse, sizeof(serverResponse));

        if (serverResponse == -1)
            printf("Insufficient Balance\n");
        else
            printf("Current Balance: %d\n", serverResponse);
    }

    close(clientSocket);
    return 0;
}
