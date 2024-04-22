#include <arpa/inet.h>    // For inet_addr()
#include <string.h>       // For memset()
#include <sys/socket.h>   // For socket API
#include <unistd.h>       // For close()
#include <stdio.h>        // For printf()
#include <sys/time.h>     // For gettimeofday()


#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"

int main(void) {
    int fd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    socklen_t addr_len;

    // Create a UDP socket
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("Socket creation failed");
        return 1;
    }
    

    // Server address configuration
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    addr_len = sizeof(server_addr);

    // User inputs a message to send
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i=0; i<1000; i++) {
        char message[256];
        snprintf(message, sizeof(message), "Package number %d", i);
        sendto(fd, message, strlen(message), 0, (struct sockaddr *)&server_addr, addr_len);
        usleep(10); // Sleep for 100ms
    }

    // Receive acknowledgment from the server and print the time elapsed
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Time elapsed: %ld seconds and %ld microseconds\n", seconds, micros);
    
    // Close the socket
    close(fd);
    return 0;
}