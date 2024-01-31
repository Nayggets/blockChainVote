#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../common/include/messages.h"

SSL_CTX* initsslcontext() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        perror("Echec lors de l'initialisation du contexte SSL");
        exit(EXIT_FAILURE);
    }
    SSL_CTX_use_certificate_file(ctx, "mycert.pem", SSL_FILETYPE_PEM);
    return ctx;
}

#define QUITTER 20

int anneeactuelle()
{
    time_t secondes=time(NULL);
    struct tm* CurrentTime=localtime(&secondes); 
    
    return (CurrentTime->tm_annee + 1900);
}

int bissextile(int an)
{
   if (an%400==0) return 1;
   if (an%100==0) return 0;
   if (an%4==0) return 1;
   return 0;
}

int longueurmois(int m,int a)
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

int datesuperieure(char* dateDebut,char* dateFin)
{
    char jour[2];
    jour[0] = dateDebut[0];
    jour[1] = dateDebut[1];
    int jourNumeroDateDebut = atoi(jour);
    char mois[2];
    strncpy(mois,&dateDebut[3],2);
    int moisNumeroDateDebut = atoi(mois); 
    char annee[4];
    annee[0] = dateDebut[6];
    annee[1] = dateDebut[7];
    annee[2] = dateDebut[8];
    annee[3] = dateDebut[9];
    int anneeNumeroDateDebut = atoi(annee);

    
    jour[0] = dateFin[0];
    jour[1] = dateFin[1];
    int jourNumeroDateFin = atoi(jour);
    strncpy(mois,&dateFin[3],2);
    int moisNumeroDateFin = atoi(mois); 
    annee[0] = dateFin[6];
    annee[1] = dateFin[7];
    annee[2] = dateFin[8];
    annee[3] = dateFin[9];
    int anneeNumeroDateFin = atoi(annee);

    if(anneeNumeroDateFin < anneeNumeroDateDebut){
        return -1;
    }
    else if(anneeNumeroDateFin > anneeNumeroDateDebut){
        return 0;
    }

    if(moisNumeroDateFin < moisNumeroDateDebut){
        return -1;
    }
    else if(moisNumeroDateFin > moisNumeroDateDebut){
        return 0;
    }

    if(jourNumeroDateFin < jourNumeroDateDebut){
        return -1;
    }
    else if(jourNumeroDateFin > jourNumeroDateDebut){
        return 0;
    }
}

int dateestok(char* date,int taille)
{
    if(taille != 10){
        printf("Taille non conforme\n");
        return -1;
    }
    for(int i = 0 ; i < taille ; i++){
        if(i != 2 && i != 5){
            if(date[i] < 48 || date[i] > 57){
                printf("La date ne doit contenir que des chiffres\n");
                return -1;
            }
        }
    }
    char jour[2];
    jour[0] = date[0];
    jour[1] = date[1];
    int jourNumero = atoi(jour);
    char mois[2];
    strncpy(mois,&date[3],2);
    int moisNumero = atoi(mois); 
    printf("%i",moisNumero);
    if(moisNumero > 12){
        printf("Rentrez un numero de mois valide %i > 12\n",atoi(mois));
        return -1;
    }
    char annee[4];
    annee[0] = date[6];
    annee[1] = date[7];
    annee[2] = date[8];
    annee[3] = date[9];
    int anneeNumero = atoi(annee);
    if(anneeNumero < anneeactuelle()){
        printf("Rentrez une année superieur a l'année actuelle %d >= %d\n",anneeNumero,anneeactuelle());
        return -1;
    }

    if(jourNumero > longueurmois(moisNumero,anneeNumero)){
        printf("Jour invalide au moins correspondant\n");
        return -1;
    }
    return 0;
}

void menu(SSL* ssl)
{
    char nombre[2];
    int verif = 0;
    Commande commande;
    while(atoi(nombre) != QUITTER)
    {
        printf("Que voulez vous faire?\n");
        printf("1. Creer electeur\n");
        printf("2. Supprimer electeur\n");
        printf("3. Verifier la presence d'un electeur\n");

        printf("4. Voter pour une election\n");
        printf("5. Update un electeur\n");
        printf("6. Lire un electeur \n");
        printf("7. AjoutElection\n");
        printf("8. Supprimer une election\n");
        printf("9. Update la question d'une election\n");
        printf("10. Lire les details d'une election\n");
        printf("11. Obtenir le resultat d'un vote\n");
        printf("20. QUITTER\n");
        verif = read(1,&nombre,2); // just pour eviter les warning du read on stock la valeur de retour
        if(verif == -1){
            perror("read");
        }

        switch(atoi(nombre))
        {
            case AJOUT_ELECTEUR:
            {
                //creer electeur
                
                printf("Entrez id/nom electeur\n");
                verif = read(1,commande.commande.ajoutElecteur.identifiant,256);
                commande.commande.ajoutElecteur.identifiant[verif-1] = '\0'; 
                commande.type = AJOUT_ELECTEUR;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case SUPPRIME_ELECTEUR:
            {
                printf("Entrez id/nom electeur\n");
                verif = read(1,commande.commande.supprimeElecteur.identifiant,256);
                commande.commande.supprimeElecteur.identifiant[verif-1] = '\0';
                // supprimer electeur
                commande.type = SUPPRIME_ELECTEUR;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case EST_PRESENT:
            {
                printf("Entrez id/nom electeur\n");
                verif = read(1,commande.commande.estPresent.identifiant,256);
                commande.commande.estPresent.identifiant[verif-1] = '\0';
                commande.type = EST_PRESENT;
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case CAST_VOTE:
            {
                //verifier electeur existe
                printf("Entrez id/nom de l'election\n");
                verif = read(1,commande.commande.castVote.idElection,256);
                commande.commande.castVote.idElection[verif-1] = '\0';
                printf("Id Election recuperer%s\n",commande.commande.castVote.idElection);
                printf("Entrez id/nom de l'electeur\n");
                verif = read(1,commande.commande.castVote.identifiant,256);
                commande.commande.castVote.identifiant[verif-1] = '\0';
                printf("option 0. ou option 1.\n");
                verif = read(1,&nombre,2); // just pour eviter les warning du read on stock la valeur de retour

                while(atoi(nombre) != 0 && atoi(nombre) != 1)
                {
                    printf("option 0. ou option 1.\n");
                    verif = read(1,&nombre,2); // just pour eviter les warning du read on stock la valeur de retour
                }
                if(atoi(nombre) == 1){
                    commande.commande.castVote.ballot = 1;
                }
                else{
                    commande.commande.castVote.ballot = 0;

                }
                commande.commande.castVote.ballotSize = 4;
                commande.type = CAST_VOTE;
                memset(commande.signature,0,256);
                SSL_write(ssl,&commande,sizeof(Commande));

                //voter
                break;
            }
            case MISE_A_JOUR_ELECTEUR:
            {
                printf("Entrez id/nom electeur\n");
                verif = read(1,commande.commande.updateElecteur.identifiant,256);
                commande.commande.updateElecteur.identifiant[verif-1] = '\0';
                printf("Entrez nouvelle id/nom electeur\n");
                verif = read(1,commande.commande.updateElecteur.nouveauId,256);
                commande.commande.updateElecteur.nouveauId[verif-1] = '\0';
                //clore le vote et avoir le resultat
                memset(commande.signature,0,256);
                commande.type = MISE_A_JOUR_ELECTEUR;
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case READ_ELECTEUR:
            {
                printf("Entrez id/nom electeur\n");
                verif = read(1,commande.commande.estPresent.identifiant,256);
                commande.commande.estPresent.identifiant[verif-1] = '\0';
                commande.type = READ_ELECTEUR;
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case AJOUT_ELECTION:
            {
                commande.type = AJOUT_ELECTION;
                printf("Entrez id/nom de l'election\n");
                verif = read(1,commande.commande.ajoutElection.identifiant,256);
                commande.commande.ajoutElection.identifiant[verif-1] = '\0';
                printf("Entrez la date de debut format (jj/mm/aaaa)\n");
                verif = read(1,commande.commande.ajoutElection.dateDebut,256);
                commande.commande.ajoutElection.dateDebut[verif-1] = '\0';
                
                while(dateestok(commande.commande.ajoutElection.dateDebut,verif-1) == -1){
                    printf("Date invalide\n");
                    printf("Entrez la date de debut format (jj/mm/aaaa)\n");
                    verif = read(1,commande.commande.ajoutElection.dateDebut,256);
                    commande.commande.ajoutElection.dateDebut[verif-1] = '\0';
                }
                printf("Entrez la date de fin format (jj/mm/aaaa)\n");
                verif = read(1,commande.commande.ajoutElection.dateFin,256);
                commande.commande.ajoutElection.dateFin[verif-1] = '\0';
                while(dateestok(commande.commande.ajoutElection.dateFin,verif-1) == -1 && datesuperieure(commande.commande.ajoutElection.dateDebut,commande.commande.ajoutElection.dateFin)){
                    printf("Date invalide\n");
                    printf("Entrez la date de fin format (jj/mm/aaaa)\n");
                    verif = read(1,commande.commande.ajoutElection.dateDebut,256);
                    commande.commande.ajoutElection.dateDebut[verif-1] = '\0';
                }
                printf("Entrez votre question fermee(oui/non)\n");
                verif = read(1,commande.commande.ajoutElection.question,256);
                commande.commande.ajoutElection.question[verif-1] = '\0';

                strcpy(commande.commande.ajoutElection.status,"active\0");

                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case SUPPRIME_ELECTION:
            {
                printf("Entrez id/nom de l'election\n");
                verif = read(1,commande.commande.supprimeElection.identifiant,256);
                commande.commande.supprimeElection.identifiant[verif-1] = '\0';
                commande.type = SUPPRIME_ELECTION;
    
                SSL_write(ssl,&commande,sizeof(Commande));
                break;
            }
            case MISE_A_JOUR_ELECTION:
            {
                printf("Entrez id/nom de l'election\n");
                verif = read(1,commande.commande.updateElection.identifiant,256);
                commande.commande.updateElection.identifiant[verif-1] = '\0';
                commande.type = MISE_A_JOUR_ELECTION;
                printf("Entrez la nouvelle question de l'election\n");
                verif = read(1,commande.commande.updateElection.question,256);
                commande.commande.updateElection.question[verif-1] = '\0';
                SSL_write(ssl,&commande,sizeof(Commande));

                break;
            }
            case READ_ELECTION:
            {
                printf("Entrez id/nom de l'election\n");
                verif = read(1,commande.commande.readElection.identifiant,256);
                commande.commande.readElection.identifiant[verif-1] = '\0';
                commande.type = READ_ELECTION;
                break;
            }
            case PROCEDER_AUX_RESULTATS_VOTE:
            {
                printf("Entrez id/nom de l'election\n");
                verif = read(1,commande.commande.procederAuxResultatsVotes.identifiantElection,256);
                commande.commande.procederAuxResultatsVotes.identifiantElection[verif-1] = '\0';
                commande.type = PROCEDER_AUX_RESULTATS_VOTE;
            }
            case QUITTER:
            {
                printf("application quittee\n");
                break;
            }
        }
    }

}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Utilisation: %s <server_address> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    SSL_CTX* ctx = initsslcontext();
    
    // Création d'une socket TCP
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd == -1) {
        perror("La creation de la socket a echouee");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    // Connexion au serveur
    if (connect(sockFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("La connexion au serveur a echouee");
        exit(EXIT_FAILURE);
    }

    // Création de la structure SSL
    SSL* ssl = SSL_new(ctx);
    if (!ssl) {
        perror("la creation de la structure ssl a echouee");
        exit(EXIT_FAILURE);
    }

    // Attachement de la socket au SSL
    if (!SSL_set_fd(ssl, sockFd)) {
        perror("l'attachement de la socket au ssl a echouee");
        exit(EXIT_FAILURE);
    }

    // Établissement de la connexion SSL sécurisée
    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
        perror("echec de la connexion ssl securisee");
        exit(EXIT_FAILURE);
    }
    menu(ssl);
    // Envoi et réception de données sécurisées
    SSL_write(ssl, message, strlen(message));

    char buffer[1024];
    SSL_read(ssl, buffer, sizeof(buffer));
    printf("Server : %s\n", buffer);

    // Fermeture de la connexion SSL et libération des ressources
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockFd);

    return 0;
}