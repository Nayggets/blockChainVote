#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../common/include/messages.h"

SSL_CTX* init_ssl_context() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        perror("Failed to initialize SSL context");
        exit(EXIT_FAILURE);
    }
    SSL_CTX_use_certificate_file(ctx, "mycert.pem", SSL_FILETYPE_PEM);
    return ctx;
}

#define QUIT 5
#define CREERELECTOR 1
#define SUPPRIMERELECTEUR 2
#define VOTEELECTEUR 3
#define CLOREVOTE 4

void menu_test(SSL* ssl)
{
    char number[2];
    int check = 0;
    Commande commande;
    while(atoi(number) != QUIT)
    {
        printf("Que voulez vous faire?\n");
        printf("1. Creer electeur\n");
        printf("2. Supprimer electeur\n");
        printf("3. Voter\n");
        printf("4. Clore un vote\n");
        printf("5. Quit\n");
        check = read(1,&number,2); // just pour eviter les warning du read on stock la valeur de retour
        if(check == -1){
            perror("read");
        }

        switch(atoi(number))
        {
            case CREERELECTOR:
            {
                //creer electeur
                
                printf("Entrez id/nom electeur\n");
                check = read(1,commande.commande.ajoutElecteur.identifiant,256);
                commande.commande.ajoutElecteur.identifiant[check] = '\0'; 
                commande.type = AJOUT_ELECTEUR;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case SUPPRIMERELECTEUR:
            {
                printf("Entrez id/nom electeur\n");
                check = read(1,commande.commande.supprimeElecteur.identifiant,256);
                commande.commande.supprimeElecteur.identifiant[check] = '\0';
                // supprimer electeur
                commande.type = SUPPRIME_ELECTEUR;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case VOTEELECTEUR:
            {
                //verifier electeur existe
                commande.type = EST_PRESENT;
                memset(commande.signature,0,256);
                //voter
                break;
            }
            case CLOREVOTE:
            {
                //clore le vote et avoir le resultat
                memset(commande.signature,0,256);
                commande.type = CAST_VOTE;
                break;
            }
            case QUIT:
            {
                printf("application quited\n");
                break;
            }
        }
    }

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
    menu_test(ssl);
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