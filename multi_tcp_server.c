#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 8080
void* handle_client(void* arg) {
    void **params = (void **)arg;
    int client_fd = (intptr_t)params[0];
    SSL_CTX *ctx = (SSL_CTX *)params[1];
    free(params);

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_fd);

    if (SSL_accept(ssl) <= 0) {
        // Enhanced error handling
        unsigned long err_code;
        while ((err_code = ERR_get_error())) {
            char *err_msg = ERR_error_string(err_code, NULL);
            fprintf(stderr, "SSL error: %s\n", err_msg);
            if (err_code == SSL_R_WRONG_VERSION_NUMBER) {
                fprintf(stderr, "Attempted connection with incompatible SSL version.\n");
                // fprintf(stderr, "Incompatible SSL version detected, closing connection.\n"); //Open this line if you want to close the connection immidiately after detecting the incompatible SSL version

            }
        }
    
        // SSL_shutdown(ssl); // Open this line if you want to close the connection immidiately after detecting the incompatible SSL version
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }

    char buffer[1024];
    int bytes_read;
    while ((bytes_read = SSL_read(ssl, buffer, sizeof(buffer)-1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Server received message: %s\n", buffer);
        SSL_write(ssl, "Hello from server", strlen("Hello from server"));
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client_fd);
    return NULL;
}

int main() {
    SSL_library_init();
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_certificate_file(ctx, "/home/ibrahim/Desktop/SSL certificates/server-cert.pem", SSL_FILETYPE_PEM) != 1 ||
        SSL_CTX_use_PrivateKey_file(ctx, "/home/ibrahim/Desktop/SSL certificates/server-key.pem", SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t addrlen = sizeof(serverAddr);  // Define addrlen here


    if (bind(server_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        int new_socket = accept(server_fd, (struct sockaddr*)&serverAddr, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        void **pclient = malloc(2 * sizeof(void*));
        pclient[0] = (void*)(intptr_t)new_socket;
        pclient[1] = (void*)ctx;

        pthread_t thread_tid;
        if (pthread_create(&thread_tid, NULL, handle_client, pclient) != 0) {
            perror("Thread creation failed");
            close(new_socket);
        } else {
            pthread_detach(thread_tid);
        }
    }

    

    SSL_CTX_free(ctx);
    close(server_fd);
    return 0;
}
