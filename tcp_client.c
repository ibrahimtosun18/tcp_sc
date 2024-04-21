#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define SIZE 1000

int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    char buffer[SIZE];

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    while(1){
        printf("Enter a message to send to the server: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character from input

        if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
            perror("Send failed");
            close(client_fd);
            return 1;
        }

        char serverResponse[SIZE];
        int len = recv(client_fd, serverResponse, SIZE, 0);
        if (len > 0) {
            serverResponse[len] = '\0';
            printf("Received data from server: %s\n", serverResponse);
        } else {
            printf("Server closed the connection.\n");
            break;
        }
    }

    close(client_fd);
    return 0;
}
