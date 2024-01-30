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
task_commande commandes[256];
int index_commandes = 0;
const int PORT = 8888;


void* worker(void*);
int fillCommand(task_commande commande);



void* receiveCommand(void* data)
{
    client_fd* client = data;
    int size = sizeof(Commande);
    Commande commande;
    while(size == sizeof(Commande)){
        task_commande task_commande;
        task_commande.commande = commande;
        task_commande.client = client;
        fillCommand(task_commande);
        size = SSL_read(client->ssl,&commande,size);

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

int fillCommand(task_commande commande)
{
    pthread_mutex_lock(&mutex);
    if(index_commandes >= 256){
        pthread_cond_wait(&cond2,&mutex);
    }
    memcpy(&commandes[index_commandes],&commande,sizeof(task_commande));
    printf("commandes ajouté avec succès type = %d",commandes[index_commandes].commande.type);
    index_commandes++;
    pthread_cond_signal(&cond);  

    pthread_mutex_unlock(&mutex);

}

task_commande* accessCommand()
{
    printf("\nCommande debut recuperation %d\n",index_commandes);

    pthread_mutex_lock(&mutex);
    if(index_commandes == 0){
        pthread_cond_wait(&cond,&mutex);
    }
    task_commande* commande = malloc(sizeof(task_commande));
    printf("Taille de commande %ld\n",sizeof(task_commande));
    memcpy(commande,&commandes[index_commandes-1],sizeof(task_commande));
    index_commandes--;
    pthread_cond_signal(&cond2);
    pthread_mutex_unlock(&mutex);
    printf("\nCommande recuperer avec succès\n");

    return commande;
}





void* worker(void* data)
{
    sqlite3* db = data;

    printf("worker lancer\n");

    while(1){
        task_commande* task_commande = accessCommand();
        printf("Traitement Commande %d\n",task_commande->commande.type);
        int code;
        switch (task_commande->commande.type)
        {
        case AJOUT_ELECTEUR:
            code = handlerajoutelecteur(db,&task_commande->commande);
            if(code == 0){
                printf("Electeur ajouter\n");
            }
            else{
                printf("Error lors de l'ajout");
            }
            break;
        case SUPPRIME_ELECTEUR:
            code = handlersupprimeElecteur(db,&task_commande->commande);
            if(code == 0){
                printf("Electeur supprimer\n");
            }
            else{
                printf("Error lors de la suppression");
            }
            break;
        case EST_PRESENT:
            code = handlerestpresent(db,&task_commande->commande);
            if(code == 0){
                printf("Electeur est present\n");
            }
            else{
                printf("Error lors du check est present");
            }
            break;
        case CAST_VOTE:
            code = handlercastvote(db,&task_commande->commande);
            if(code == 0){
                printf("Vote a fonctionner\n");
            }
            else{
                printf("Error lors du vote\n");
            }
            break;
        case UPDATE_ELECTEUR:
            if(handlerupdateelecteur(db,&task_commande->commande) == 0){
                printf("Update a fonctionner\n");
            }
            else{
                printf("Error lors de l'update\n");
            }
            break;
        case READ_ELECTEUR:
            if(handlerReadElecteur(db,&task_commande->commande) == 0){
                printf("Read election\n");
            }
            else{
                printf("Error lors du read\n");
            }
            break;
        case AJOUT_ELECTION:
            code = handlerAjoutelection(db,&task_commande->commande);
            if(code == 0){
                printf("Election creer avec succès\n");
            } 
            else{
                printf("Error lors de la creation d'election\n");
            }
            break;
        case SUPPRIME_ELECTION:
            if(handlerSupprimeElection(db,&task_commande->commande) == 0){
                printf("Supprimer Election\n");
            }
            else{
                printf("Supprimer election\n");
            }
            break;
        case UPDATE_ELECTION:
            if(handlerUpdateElection(db,&task_commande->commande)){
                printf("Update Election\n");
            }
            else{
                printf("Update Election\n");
            }
            break;
        case READ_ELECTION:
            if(handlerReadElection(db,&task_commande->commande)){
                printf("Read election\n");
            }
            else{
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
        free(task_commande);
    }
}