#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define PORT 8080
#define SIZE 1000

int main(int argc, char *argv[]) {
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

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Client identifier passed via command-line argument
    char* client_id = "Unknown";
    if (argc > 1) {
        client_id = argv[1];
    }

    // Message sending loop
    for (int i = 0; i < 1000; i++) {
        char message[256];
        snprintf(message, sizeof(message), "Client %s: Package number %d", client_id, i);
        if (send(client_fd, message, strlen(message), 0) < 0) {
            perror("Send failed");
            break;
        }
        usleep(100000); // Sleep for 10 microseconds
    }

    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Time elapsed: %ld seconds and %ld microseconds\n", seconds, micros);

    close(client_fd);
    return 0;
}