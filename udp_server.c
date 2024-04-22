#include <arpa/inet.h>  // For byte order conversions and inet_ntoa()
#include <string.h>     // For memset()
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>      // For printf()

#define BUFFER_SIZE 1024
#define SERVER_PORT 12345

int main(void) {
    
    int fd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in address, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    // Create a IP4/UDP socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // memset is used to clear the memory of the address structure
    memset(&address, 0, sizeof(address));

    // Server address configuration
    address.sin_family = AF_INET;
    address.sin_port = htons(SERVER_PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(fd);
        return 1;
    }

    //tell the user that the server is ready to receive messages
    printf("Server is ready to receive messages\n");

    while (1) {
        // Receive the message from the client
        int len = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cli_addr, &cli_len);

        if (len > 0) {
            buffer[len] = '\0';  // Null-terminate the received data
            printf("Server received message \"%s\" from %s\n", buffer, inet_ntoa(cli_addr.sin_addr));

            // Send acknowledgment back to the client
            char ack_message[BUFFER_SIZE];
            snprintf(ack_message, BUFFER_SIZE, "Server received message \"%s\" from %s", buffer, inet_ntoa(cli_addr.sin_addr));
            sendto(fd, ack_message, strlen(ack_message), 0, (struct sockaddr *)&cli_addr, cli_len);
        }
    }

    close(fd);
    return 0;
}