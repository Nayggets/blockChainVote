#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

SSL_CTX* init_ssl_context() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        perror("Failed to initialize SSL context");
        exit(EXIT_FAILURE);
    }
    return ctx;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_address> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SSL_CTX* ctx = init_ssl_context();
    
    // Création d'une socket TCP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    // Création de la structure SSL
    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        perror("Failed to create SSL structure");
        exit(EXIT_FAILURE);
    }

    // Attachement de la socket au SSL
    if (!SSL_set_fd(ssl, sockfd)) {
        perror("Failed to attach socket to SSL");
        exit(EXIT_FAILURE);
    }

    // Établissement de la connexion SSL sécurisée
    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
        perror("SSL handshake failed");
        exit(EXIT_FAILURE);
    }

    // Envoi et réception de données sécurisées
    const char* message = "Zizon connecté";
    SSL_write(ssl, message, strlen(message));

    char buffer[1024];
    SSL_read(ssl, buffer, sizeof(buffer));
    printf("Server : %s\n", buffer);

    // Fermeture de la connexion SSL et libération des ressources
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockfd);

    return 0;
}