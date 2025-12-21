#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    int choice, amount, result;

    printf("1. Withdraw\n2. Deposit\n3. Display Balance\n4. Exit\n");
    scanf("%d", &choice);

    write(sock, &choice, sizeof(choice));

    if (choice == 1 || choice == 2) {
        printf("Enter amount: ");
        scanf("%d", &amount);
        write(sock, &amount, sizeof(amount));
    }

    if (choice != 4) {
        read(sock, &result, sizeof(result));
        if (result == -1)
            printf("Insufficient Balance\n");
        else
            printf("Current Balance: %d\n", result);
    }

    close(sock);
}
