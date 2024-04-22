#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[1024];
    int bytes_read;

    while ((bytes_read = read(client_fd, buffer, sizeof(buffer)-1)) > 0) 
    {
        buffer[bytes_read] = '\0'; // Null terminate the string
        printf("Server received message: %s from port %d\n", buffer, PORT);
        write(client_fd, "Hello from server", 17);
    }
    

    close(client_fd);
    return NULL;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in serverAddr;
    int addrlen = sizeof(serverAddr);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) 
    {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr*)&serverAddr, (socklen_t*)&addrlen);
        if (new_socket < 0) 
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        pthread_t thread_tid;
        int* client_fd = (int*)malloc(sizeof(int));
        *client_fd = new_socket;

        if (pthread_create(&thread_tid, NULL, handle_client, client_fd) != 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }

        pthread_detach(thread_tid);

    }

    close(server_fd);
    return 0;
}