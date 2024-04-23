#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8080
#define SIZE 1000

int main(int argc, char *argv[]) {
    // Initialize SSL
    SSL_library_init();
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();

    // Create an SSL context
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Load trust store
    if (!SSL_CTX_load_verify_locations(ctx, "/home/ibrahim/Desktop/SSL certificates/ca-cert.pem", NULL))
    {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    // Create an SSL object and attach it to the socket
    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);

    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(client_fd);
        SSL_CTX_free(ctx);
        exit(EXIT_FAILURE);
    }

    char buffer[SIZE];
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Client identifier passed via command-line argument
    char* client_id = "Unknown";
    if (argc > 1) {
        client_id = argv[1];
    }

    // Message sending loop
    for (int i = 0; i < 500; i++) {
        char message[256];
        snprintf(message, sizeof(message), "Client %s: Package number %d", client_id, i);
        if (SSL_write(ssl, message, strlen(message)) <= 0) {
            perror("SSL Send failed");
            break;
        }
        usleep(100000); // Sleep for 100 milliseconds
    }

    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Time elapsed: %ld seconds and %ld microseconds\n", seconds, micros);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client_fd);
    SSL_CTX_free(ctx);
    return 0;
}
