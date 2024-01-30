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
int fillCommand(Commande commande);



void* receiveCommand(void* data)
{
    client_fd* client = data;
    int size = sizeof(Commande);
    char buff[sizeof(Commande)];
    while(size == sizeof(Commande)){
        Commande* commande;
        commande = (Commande*)&buff[0];
        fillCommand(*commande);
        size = SSL_read(client->ssl,buff,size);

    }
    printf("Client left\n");
    close(client->fd);
    SSL_free(client->ssl);
}


void launch(void* data)
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

    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
    SSL_CTX *ctx;
    //SSL *ssl;   
 

    /* Create a TLS server context with certificates */

    ctx = SSL_CTX_new(TLS_server_method());

    SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM);

    SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM);

 
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
   
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
    SSL* ssl;
    client_fd* client;
    // Accept the data packet from client and verification 
    int check = 0;
    while(1)
    {
        client = malloc(sizeof(client_fd));
        connfd = accept(sockfd, (struct sockaddr *)&cli,(socklen_t*) &len); 
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
                printf("server accept the client...\n");
                pthread_t pthread;
                pthread_create(&pthread,NULL,receiveCommand,client);

            }
            else{
                printf("Problème lié a ssl abandon du client\n");
                close(connfd);
                SSL_free(ssl);

    
            }


        }

    }

    
}

int fillCommand(Commande commande)
{
    pthread_mutex_lock(&mutex);
    if(index_commandes >= 256){
        pthread_cond_wait(&cond2,&mutex);
    }
    memcpy(&commandes[index_commandes],&commande,sizeof(Commande));
    printf("commandes ajouté avec succès type = %d",commandes[index_commandes].type);
    index_commandes++;
    pthread_cond_signal(&cond);  

    pthread_mutex_unlock(&mutex);

}

Commande* accessCommand()
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
    printf("\nCommande recuperer avec succès\n");

    return commande;
}


void chaine_err(CommandType code_action,CodeErreur code_erreur,char* chaine_a_remplir)
{
  if(code_action == AJOUT_ELECTEUR && code_erreur == REUSSITE){
    strcpy(chaine_a_remplir,"Reussite lors de l'ajout de l'electeur");
  }
  else if (code_action == AJOUT_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de l'ajout de l'electeur");
  }
  else if (code_action == SUPPRIME_ELECTEUR && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la suppression");
  }
  else if (code_action == SUPPRIME_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de la suppression de l'electeur");
  }
  else if (code_action == EST_PRESENT && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la vérification de la présence de l'electeur");
  }
  else if (code_action == EST_PRESENT && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de la vérification de la présence de l'electeur");
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
  else if (code_action == UPDATE_ELECTEUR && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la mise a jour de l'electeur");
  }
  else if (code_action == UPDATE_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors de la mise a jour de l'electeur");
  }
  else if (code_action == READ_ELECTEUR && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors du read");
  }
  else if (code_action == READ_ELECTEUR && code_erreur == ELECTEUR_PAS_PRESENT)
  {
    strcpy(chaine_a_remplir,"Echec lors du read");
  }
  else if (code_action == AJOUT_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de l'ajout de l'election");
  }
  else if (code_action == AJOUT_ELECTION && code_erreur == ElECTION_EXISTE_DEJA)
  {
    strcpy(chaine_a_remplir,"Echec lors de l'ajout de l'election");
  }
  else if (code_action == SUPPRIME_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la suppression de l'élection");
  }
  else if (code_action == SUPPRIME_ELECTION && code_erreur == ElECTION_PAS_PRESENTE)
  {
    strcpy(chaine_a_remplir,"Echec lors de la suppression de l'élection");
  }
  else if (code_action == UPDATE_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors de la mise à jour de l'élection");
  }
  else if (code_action == UPDATE_ELECTION && code_erreur == ElECTION_PAS_PRESENTE)
  {
    strcpy(chaine_a_remplir,"Echec lors de la mise à jour de l'élection");
  }
  else if (code_action == READ_ELECTION && code_erreur == REUSSITE)
  {
    strcpy(chaine_a_remplir,"Reussite lors du read de l'election");
  }
  else if (code_action == READ_ELECTION && code_erreur == ElECTION_PAS_PRESENTE)
  {
    strcpy(chaine_a_remplir,"Echec lors du read de l'election");
  }
// reste proccesss vote a faire mais j'ai pas le handler ducoup

}


void* worker(void* data)
{
    sqlite3* db = data;

    printf("worker lancer\n");

    while(1){
        Commande* commande = accessCommand();
        printf("Traitement Commande %d\n",commande->type);
        int code;
        switch (commande->type)
        {
        case AJOUT_ELECTEUR:
            code = handlerajoutelecteur(db,commande);
            if(code == REUSSITE){
                chaine_err(AJOUT_ELECTEUR,code,"Super !");
                printf("Electeur ajouter\n");
            }
            else{
                chaine_err(AJOUT_ELECTEUR,code,"il manque un electeur");
                printf("Error lors de l'ajout");
            }
            break;
        case SUPPRIME_ELECTEUR:
            code = handlersupprimeElecteur(db,commande);
            if(code == REUSSITE){
                chaine_err(SUPPRIME_ELECTEUR,code,"Super !");
                printf("Electeur supprimer\n");
            }
            else{
                chaine_err(SUPPRIME_ELECTEUR,code,"Il manque un electeur ");
                printf("Error lors de la suppression");
            }
            break;
        case EST_PRESENT:
            code = handlerestpresent(db,commande);
            if(code == REUSSITE){
                chaine_err(EST_PRESENT,code,"Super !");
                printf("Electeur est present\n");
            }
            else{
                chaine_err(EST_PRESENT,code,"Il manque un electeur");
                printf("Error lors du check est present");
            }
            break;
        case CAST_VOTE:
            code = handlercastvote(db,commande);
            if(code == REUSSITE){
                chaine_err(CAST_VOTE,code,"Super !");
                printf("Vote a fonctionner\n");
            }
            else{
                chaine_err(CAST_VOTE,code,"Erreur !");
                printf("Error lors du vote\n");
            }
            break;
        case UPDATE_ELECTEUR:
            code = handlerupdateelecteur(db,commande);
            if(code == REUSSITE){
                chaine_err(UPDATE_ELECTEUR,code,"Super !");
                printf("Update a fonctionner\n");
            }
            else{
                chaine_err(UPDATE_ELECTEUR,code,"l'électeur n'éxiste pas");
                printf("Error lors de l'update\n");
            }
            break;
        case READ_ELECTEUR:
            code = handlerReadElecteur(db,commande);
            if(code == REUSSITE){
                chaine_err(READ_ELECTEUR,code,"Super !");
                printf("Read election\n");
            }
            else{
                chaine_err(READ_ELECTEUR,code,"L'électeur n'éxiste pas");
                printf("Error lors du read\n");
            }
            break;
        case AJOUT_ELECTION:
            code = handlerAjoutelection(db,commande);
            if(code == REUSSITE){
                chaine_err(AJOUT_ELECTION,code,"Super !");
                printf("Election creer avec succès\n");
            } 
            else{
                chaine_err(AJOUT_ELECTION,code,"L'élection n'éxiste pas ");
                printf("Error lors de la creation d'election\n");
            }
            break;
        case SUPPRIME_ELECTION:
            code = handlerSupprimeElection(db,commande);
            if(code == REUSSITE){
                chaine_err(SUPPRIME_ELECTION,code,"Super !");
                printf("Supprimer Election\n");
            }
            else{
                chaine_err(SUPPRIME_ELECTION,code,"L'élection n'éxiste pas ");
                printf("Supprimer election\n");
            }
            break;
        case UPDATE_ELECTION:
            code = handlerUpdateElection(db,commande);
            if(code == REUSSITE){
                chaine_err(UPDATE_ELECTION,code,"Super !");
                printf("Update Election\n");
            }
            else{
                chaine_err(UPDATE_ELECTION,code,"L'élection n'éxiste pas ");
                printf("Update Election\n");
            }
            break;
        case READ_ELECTION:
            code =handlerReadElection(db,commande);
            if(code == REUSSITE){
                chaine_err(READ_ELECTION,code,"Super !");
                printf("Read election\n");
            }
            else{
                chaine_err(READ_ELECTION,code,"L'élection n'éxiste pas ");
                printf("Read election\n");
            }
            break;
        case PROCESS_VOTES:
            //if()

            break;
        default:
            printf("Commande inconnu abandon du traitement.\n");
            break;
        }
        free(commande);
    }
}