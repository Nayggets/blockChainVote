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

#define QUIT 20

int actual_year()
{
    time_t seconds=time(NULL);
    struct tm* current_time=localtime(&seconds); 
    
    return (current_time->tm_year + 1900);
}

int bissextile(int an)
{
   if (an%400==0) return 1;
   if (an%100==0) return 0;
   if (an%4==0) return 1;
   return 0;
}

int longueur_mois(int m,int a)
{
    switch(m)
    {
    case 1: return 31;
    case 2:
         if (bissextile(a))
         return 29;
         else
         return 28;
    case 3: return 31;
    case 4: return 30;
    case 5: return 31;
    case 6: return 30;
    case 7: return 31;
    case 8: return 31;
    case 9: return 30;
    case 10: return 31;
    case 11: return 30;
    case 12: return 31;
    default: return 0;
    }
}

int date_superior(char* dateDebut,char* dateFin)
{
    char day[2];
    day[0] = dateDebut[0];
    day[1] = dateDebut[1];
    int day_number_debut = atoi(day);
    char month[2];
    strncpy(month,&dateDebut[3],2);
    int month_number_debut = atoi(month); 
    char year[4];
    year[0] = dateDebut[6];
    year[1] = dateDebut[7];
    year[2] = dateDebut[8];
    year[3] = dateDebut[9];
    int year_number_debut = atoi(year);

    
    day[0] = dateFin[0];
    day[1] = dateFin[1];
    int day_number_fin = atoi(day);
    strncpy(month,&dateFin[3],2);
    int month_number_fin = atoi(month); 
    year[0] = dateFin[6];
    year[1] = dateFin[7];
    year[2] = dateFin[8];
    year[3] = dateFin[9];
    int year_number_fin = atoi(year);

    if(year_number_fin < year_number_debut){
        printf("1");
        return -1;
    }
    else if(year_number_fin > year_number_debut){
                printf("2");

        return 0;
    }

    if(month_number_fin < month_number_debut){
                printf("3");

        return -1;
    }
    else if(month_number_fin > month_number_debut){
                printf("4");

        return 0;
    }

    if(day_number_fin < day_number_debut){
                printf("5");

        return -1;
    }
    else if(day_number_fin > day_number_debut){
                printf("6");

        return 0;
    }

    return 0;
}

int date_is_ok(char* date,int size)
{
    if(size != 10){
        printf("Taille non conforme\n");
        return -1;
    }
    for(int i = 0 ; i < size ; i++){
        if(i != 2 && i != 5){
            if(date[i] < 48 || date[i] > 57){
                printf("La date ne doit contenir que des chiffres\n");
                return -1;
            }
        }
    }
    char day[2];
    day[0] = date[0];
    day[1] = date[1];
    int day_number = atoi(day);
    char month[2];
    strncpy(month,&date[3],2);
    int month_number = atoi(month); 
    printf("%i",month_number);
    if(month_number > 12){
        printf("Rentrez un numero de mois valide %i > 12\n",atoi(month));
        return -1;
    }
    char year[4];
    year[0] = date[6];
    year[1] = date[7];
    year[2] = date[8];
    year[3] = date[9];
    int year_number = atoi(year);
    if(year_number < actual_year()){
        printf("Rentrez une année superieur a l'année actuel %d >= %d\n",year_number,actual_year());
        return -1;
    }

    if(day_number > longueur_mois(month_number,year_number)){
        printf("Jour invalid au moins correspondant\n");
        return -1;
    }
    return 0;
}

void read_message_correction(paquets paquet)
{
    if(paquet.codeErreur){
        printf("\033[31m");
    }   
    else{
        printf("\033[32m");
    }
    printf("%s\n",paquet.message);

    printf("\033[0m");
}

#define STATUS 6

void menu_test(SSL* ssl)
{
    char number[3];
    int check = 0;
    Commande commande;
    ListCommande listCommande;
    char* list;
    paquets paquet;
    while(atoi(number) != QUIT)
    {
        printf("Rentrez un chiffre pour executez une commande.\n");
        printf("|********************************************|\n");
        printf("| 1.Creer electeur                           |\n");
        printf("| 2.Supprimer electeur                       |\n");
        printf("| 3.Verifier la presence d'un electeur       |\n");
        printf("| 4.Voter pour une election                  |\n");
        printf("| 5.Update un electeur                       |\n");
        printf("| 6.Cancel une election                      |\n");
        printf("| 7.AjoutElection                            |\n");
        printf("| 8.Supprimer une election                   |\n");
        printf("| 9.Update la question d'une election        |\n");
        printf("| 10.Lire les details d'une election         |\n");
        printf("| 11.Obtenir le resultat d'un vote           |\n");
        printf("| 13.Lister Election                         |\n");
        printf("| 20.Quit                                    |\n");
        printf("|********************************************|\n");
        check = read(1,&number,3); // just pour eviter les warning du read on stock la valeur de retour
        if(check == -1){
            perror("read");
        }
        number[check-1] = '\0';

        switch(atoi(number))
        {
            
            case AJOUT_ELECTEUR:
            {
                //creer electeur
                printf("Entrez id/nom electeur\n");
                check = read(1,commande.commande.ajoutElecteur.identifiant,256);
                commande.commande.ajoutElecteur.identifiant[check-1] = '\0'; 
                commande.type = AJOUT_ELECTEUR;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));
                
                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case SUPPRIME_ELECTEUR:
            {
                printf("Entrez id/nom electeur\n");
                check = read(1,commande.commande.supprimeElecteur.identifiant,256);
                commande.commande.supprimeElecteur.identifiant[check-1] = '\0';
                // supprimer electeur
                commande.type = SUPPRIME_ELECTEUR;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case EST_PRESENT:
            {
                printf("Entrez id/nom electeur\n");
                check = read(1,commande.commande.estPresent.identifiant,256);
                commande.commande.estPresent.identifiant[check-1] = '\0';
                commande.type = EST_PRESENT;
                SSL_write(ssl,&commande,sizeof(Commande));
                
                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case CAST_VOTE:
            {
                //verifier electeur existe
                printf("Entrez id/nom de l'election\n");
                check = read(1,commande.commande.castVote.idElection,256);
                commande.commande.castVote.idElection[check-1] = '\0';
                printf("Id Election recuperer%s\n",commande.commande.castVote.idElection);
                printf("Entrez id/nom de l'electeur\n");
                check = read(1,commande.commande.castVote.identifiant,256);
                commande.commande.castVote.identifiant[check-1] = '\0';
                printf("option 0. ou option 1.\n");
                check = read(1, &number, 2); // juste pour eviter les avertissements du read on stock la valeur de retour

                while(atoi(number) != 0 && atoi(number) != 1)
                {
                    printf("option 0. ou option 1.\n");
                    check = read(1,&number,2); // juste pour eviter les avertissements du read on stock la valeur de retour
                }
                if(atoi(number) == 1){
                    commande.commande.castVote.ballot = 1;
                }
                else{
                    commande.commande.castVote.ballot = 0;

                }
                commande.commande.castVote.ballotSize = 4;
                commande.type = CAST_VOTE;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&paquet,sizeof(paquets));
                //voter
                break;
            }
            case UPDATE_ELECTEUR:
            {
                printf("Entrez id/nom electeur\n");
                check = read(1,commande.commande.updateElecteur.identifiant,256);
                commande.commande.updateElecteur.identifiant[check-1] = '\0';
                printf("Entrez nouvelle id/nom electeur\n");
                check = read(1,commande.commande.updateElecteur.newId,256);
                commande.commande.updateElecteur.newId[check-1] = '\0';
                //clore le vote et avoir le resultat
                memset(commande.signature,0,256);
                commande.type = UPDATE_ELECTEUR;
                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case STATUS:
            {
                printf("Entrez id/nom election\n");
                check = read(1,commande.commande.updateStatus.identifiant,256);
                commande.commande.updateStatus.identifiant[check-1] = '\0';
                commande.type = UPDATE_STATUS;
                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case AJOUT_ELECTION:
            {
                commande.type = AJOUT_ELECTION;
                printf("Entrer id/nom de l'election\n");
                check = read(1,commande.commande.ajoutElection.identifiant,256);
                commande.commande.ajoutElection.identifiant[check-1] = '\0';
                printf("Entrer la date de debut format (jj/mm/aaaa)\n");
                check = read(1,commande.commande.ajoutElection.dateDebut,256);
                commande.commande.ajoutElection.dateDebut[check-1] = '\0';
                
                while(date_is_ok(commande.commande.ajoutElection.dateDebut,check-1) == -1){
                    printf("Date invalid\n");
                    printf("Entrer la date de debut format (jj/mm/aaaa)\n");
                    check = read(1,commande.commande.ajoutElection.dateDebut,256);
                    commande.commande.ajoutElection.dateDebut[check-1] = '\0';
                }
                printf("Entrer la date de fin format (jj/mm/aaaa)\n");
                check = read(1,commande.commande.ajoutElection.dateFin,256);
                commande.commande.ajoutElection.dateFin[check-1] = '\0';
                while(date_is_ok(commande.commande.ajoutElection.dateFin,check-1) == -1 || date_superior(commande.commande.ajoutElection.dateDebut,commande.commande.ajoutElection.dateFin) == -1){
                    printf("Date invalid\n");
                    printf("Entrer la date de fin format (jj/mm/aaaa)\n");
                    check = read(1,commande.commande.ajoutElection.dateDebut,256);
                    commande.commande.ajoutElection.dateDebut[check-1] = '\0';
                }
                printf("Entrez votre question fermer(oui/non)\n");
                check = read(1,commande.commande.ajoutElection.question,256);
                commande.commande.ajoutElection.question[check-1] = '\0';

                strcpy(commande.commande.ajoutElection.status,"active\0");

                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case SUPPRIME_ELECTION:
            {
                printf("Entrez id/nom de l'election\n");
                check = read(1,commande.commande.supprimeElection.identifiant,256);
                commande.commande.supprimeElection.identifiant[check-1] = '\0';
                commande.type = SUPPRIME_ELECTION;
    
                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case UPDATE_ELECTION:
            {
                printf("Entrez id/nom de l'election\n");
                check = read(1,commande.commande.updateElection.identifiant,256);
                commande.commande.updateElection.identifiant[check-1] = '\0';
                commande.type = UPDATE_ELECTION;
                printf("Entrez la nouvelle question de l'election\n");
                check = read(1,commande.commande.updateElection.question,256);
                commande.commande.updateElection.question[check-1] = '\0';
                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case READ_ELECTION:
            {
                printf("Entrez id/nom de l'election\n");
                check = read(1,commande.commande.readElection.identifiant,256);
                commande.commande.readElection.identifiant[check-1] = '\0';
                commande.type = READ_ELECTION;
                SSL_write(ssl,&commande,sizeof(Commande));
                SSL_read(ssl,&paquet,sizeof(paquets));

                break;
            }
            case PROCESS_VOTES:
            {
                printf("Entrez id/nom de l'election\n");
                check = read(1,commande.commande.processVotes.identifiantElection,256);
                commande.commande.processVotes.identifiantElection[check-1] = '\0';
                commande.type = PROCESS_VOTES;

                SSL_write(ssl,&commande,sizeof(Commande));
                SSL_read(ssl,&paquet,sizeof(paquets));
                break;
            }
            case LIST_ELECTION:
            {
                        
                commande.type = LIST_ELECTION;

                SSL_write(ssl,&commande,sizeof(Commande));

                SSL_read(ssl,&listCommande,sizeof(ListCommande));
                printf("%d size \n",listCommande.size);
                if(listCommande.size != 0){
                    list = malloc(sizeof(char) * listCommande.size);
                    SSL_read(ssl,list,listCommande.size);
                    list[listCommande.size] = '\0';
                    printf("Liste des elections : %s\n",list);
                    free(list);

                }
                SSL_read(ssl,&paquet,sizeof(paquets));
                
                break;
            }
            case QUIT:
            {
                printf("application quited\n");
                return;
            }
            
        }
        read_message_correction(paquet);
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

    // Fermeture de la connexion SSL et libération des ressources
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockfd);

    return 0;
}