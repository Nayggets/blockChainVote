#include "../common/include/votechain.h"
#include "../common/include/crypto.h"
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../common/include/handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
mpz_t n, lambda, g, mu;
Commande commandes[256];
int index_commandes = 0;
const int PORT = 8888;


void* worker(void*);
int remplircommande(Commande commande);



void* recevoircommand(void* data)
{
    ClientFd* client = data;
    int size = sizeof(Commande);
    char buff[sizeof(Commande)];
    while(size == sizeof(Commande)){
        Commande* commande;
        commande = (Commande*)&buff[0];
        remplircommande(*commande);
        size = SSL_read(client->ssl,buff,size);

    }
    printf("Client a quitte\n");
    close(client->fd);
    SSL_free(client->ssl);
}


void lancement(void* data)
{
    mpz_inits(g, mu,NULL);
    generate_keys(n, lambda, g, mu);

    int needInit = database_exists("exemple.db");
    struct stat buffer;
    sqlite3 *db = database_open("exemple.db");
    g_assert_nonnull(db);
    if (needInit == -1)
    {
        g_print("initilisation de la base de données");
        database_init(db);
    }
    int n = *(int*)(data);
    pthread_t* t =malloc(sizeof(pthread_t) * n);
    for(int i = 0 ; i < n ;i++){
        pthread_create(&t[i],NULL,worker,db);
    }

    int sockFd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
    SSL_CTX *ctx;
    //SSL *ssl;   
 

    /* Create a TLS server context with certificates */

    ctx = SSL_CTX_new(TLS_server_method());

    SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);

    SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);

 
    // socket create and verification 
    sockFd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockFd == -1) { 
        printf("erreur lors de la creation de la socket...\n"); 
        exit(0); 
    } 
    else
        printf("la socket a bien ete cree..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
   
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockFd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        printf("echec lors de la liaison de la socket...\n"); 
        exit(0); 
    } 
    else
        printf("La socket a bien ete liee..\n"); 
   
    // Now server is ready to listen and verification 
    if ((listen(sockFd, 5)) != 0) { 
        printf("Erreur lors de l'ecoute...\n"); 
        exit(0); 
    } 
    else
        printf("Serveur en ecoute..\n"); 
    len = sizeof(cli); 
    SSL* ssl;
    ClientFd* client;
    // Accept the data packet from client and verification 
    int check = 0;
    while(1)
    {
        client = malloc(sizeof(ClientFd));
        connfd = accept(sockFd, (struct sockaddr *)&cli,(socklen_t*) &len); 
        ssl = SSL_new(ctx);

        SSL_set_fd(ssl, connfd);
        check = SSL_accept(ssl);

        if (connfd < 0) { 
            printf("Server accept failed...\n"); 
            exit(0); 
        } 
        else{
            if(check == 1){
                client->ssl = ssl;
                client->fd = connfd;
                printf("Le serveur a accepter le client...\n");
                pthread_t pthread;
                pthread_create(&pthread,NULL,recevoircommand,client);

            }
            else{
                printf("Probleme lie a ssl abandon du client\n");
                close(connfd);
                SSL_free(ssl);

    
            }


        }

    }

    
}

int remplircommande(Commande commande)
{
    pthread_mutex_lock(&mutex);
    if(index_commandes >= 256){
        pthread_cond_wait(&cond2,&mutex);
    }
    memcpy(&commandes[index_commandes],&commande,sizeof(Commande));
    printf("commandes ajoute avec succes type = %d",commandes[index_commandes].type);
    index_commandes++;
    pthread_cond_signal(&cond);  

    pthread_mutex_unlock(&mutex);

}

Commande* accedercommand()
{
    printf("\nCommande debut recuperation %d\n",index_commandes);

    pthread_mutex_lock(&mutex);
    if(index_commandes == 0){
        pthread_cond_wait(&cond,&mutex);

    }
    Commande* commande = malloc(sizeof(Commande));
    printf("Taille de commande %ld\n",sizeof(Commande));
    memcpy(commande,&commandes[index_commandes-1],sizeof(Commande));
    index_commandes--;
    pthread_cond_signal(&cond2);
    pthread_mutex_unlock(&mutex);
    printf("\nCommande recuperer avec succes\n");

    return commande;
}


void chaineerreur(CommandeType code_action,CodeErreur code_erreur,char* chaine_a_remplir)
{
  if(code_action == AJOUT_ELECTEUR && code_erreur == REUSSITE){
    strcpy(chaine_a_remplir,"Reussite lors de l'ajout de l'electeur");
  }
  else if (code_action == AJOUT_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de l'ajout de l'electeur, l'electeur existe deja");
  }
  else if (code_action == SUPPRIME_ELECTEUR && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la suppression");
  }
  else if (code_action == SUPPRIME_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de la suppression de l'electeur,l'electeur n'existe plus");
  }
  else if (code_action == EST_PRESENT && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la vérification de la présence de l'electeur");
  }
  else if (code_action == EST_PRESENT && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de la vérification de la présence de l'electeur,l'electeur n'existe pas");
  } 
  else if (code_action == CAST_VOTE && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de l'ajout de l'electeur à l'election");
  }
  else if (code_action == CAST_VOTE && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec l'électeur n'est pas présent");
  }
  else if (code_action == CAST_VOTE && code_erreur == ElECTION_PAS_PRESENTE)
  {
    strcpy(chaine_a_remplir,"Echec l'élection n'est pas présente");
  }
  else if (code_action == MISE_A_JOUR_ELECTEUR && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la mise a jour de l'electeur");
  }
  else if (code_action == MISE_A_JOUR_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de la mise a jour de l'electeur,l'electeur n'existe pas");
  }
  else if (code_action == READ_ELECTEUR && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors du read");
  }
  else if (code_action == READ_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors du read,l'electeur n'existe pas");
  }
  else if (code_action == AJOUT_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de l'ajout de l'election");
  }
  else if (code_action == AJOUT_ELECTION && code_erreur == ElECTION_EXISTE_DEJA)
  {
    strcpy(chaine_a_remplir,"Echec lors de l'ajout de l'election, l'election existe deja");
  }
  else if (code_action == SUPPRIME_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la suppression de l'élection");
  }
  else if (code_action == SUPPRIME_ELECTION && code_erreur == ElECTION_PAS_PRESENTE)
  {
    strcpy(chaine_a_remplir,"Echec lors de la suppression de l'élection, l'election n'existe pas");
  }
  else if (code_action == MISE_A_JOUR_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la mise à jour de l'élection");
  }
  else if (code_action == MISE_A_JOUR_ELECTION && code_erreur == ElECTION_PAS_PRESENTE)
  {
    strcpy(chaine_a_remplir,"Echec lors de la mise à jour de l'élection, l'election n'existe pas");
  }
  else if (code_action == READ_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors du read de l'election");
  }
  else if (code_action == READ_ELECTION && code_erreur == ElECTION_PAS_PRESENTE)
  {
    strcpy(chaine_a_remplir,"Echec lors du read de l'election, l'election n'existe pas");
  }
// reste proccesss vote a faire mais j'ai pas le handler ducoup

}


void* worker(void* data)
{
    sqlite3* db = data;

    printf("worker lance\n");
    Paquets to_client;
    
    while(1){
        Commande* commande = accedercommand();
        printf("Traitement Commande %d\n",commande->type);
        to_client.command = commande->type;
        switch (commande->type)
        {
        case AJOUT_ELECTEUR:
            to_client.codeErreur = handlerajoutelecteur(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Electeur ajoute\n");
            }
            else{
                printf("Erreur lors de l'ajout");
            }
            break;
        case SUPPRIME_ELECTEUR:
            to_client.codeErreur = handlersupprimeelecteur(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Electeur supprimer\n");
            }
            else{
                printf("Erreur lors de la suppression");
            }
            break;
        case EST_PRESENT:
            to_client.codeErreur = handlerestpresent(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Electeur est present\n");
            }
            else{
                printf("Erreur lors du check est present");
            }
            break;
        case CAST_VOTE:
            to_client.codeErreur = handlercastvote(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Vote a fonctionne\n");
            }
            else{
                printf("Erreur lors du vote\n");
            }

            break;
        case MISE_A_JOUR_ELECTEUR:
            to_client.codeErreur = handlermiseajourelecteur(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Mise a jour a fonctionne\n");
            }
            else{
                printf("Erreur lors de l'update\n");
            }

            break;
        case READ_ELECTEUR:
            to_client.codeErreur = handlerreadelecteur(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Read election\n");
            }
            else{
                printf("Erreur lors du read\n");
            }

            break;
        case AJOUT_ELECTION:
            to_client.codeErreur = handlerajouteelection(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Election cree avec succes\n");
            } 
            else{
                printf("Erreur lors de la creation d'election\n");
            }
            break;
        case SUPPRIME_ELECTION:
            to_client.codeErreur = handlersupprimeelection(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Election supprimee avec succes\n");
            }
            else{
                printf("Erreur lors de la supression de l'election\n");
            }

            break;
        case MISE_A_JOUR_ELECTION:
            to_client.codeErreur = handlermiseajourelection(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Succes lors de la mise a jour de l'election\n");
            }
            else{
                printf("Erreur lors de la mise a jour de l'election\n");
            }

            break;
        case READ_ELECTION:
            to_client.codeErreur =handlerreadelection(db,commande);
            if(to_client.codeErreur == REUSSITE){
                printf("Succes lors du read de l'election\n");
            }
            else{
                printf("Erreur lors du read de l'election\n");
            }

            break;
        case PROCEDER_AUX_RESULTATS_VOTE:
            //if()

            break;
        default:
            to_client.codeErreur = 0xff;
            to_client.command = 0xff;
            printf("Commande inconnu abandon du traitement.\n");
            break;
        }
        if(to_client.codeErreur != 0xff && to_client.command != 0xff){
            SSL_write(TacheCommande)
        }
        
        free(commande);
    }
}