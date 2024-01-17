#include "../common/include/votechain.h"
#include "./handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <glib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
Commande commandes[256];
int index_commandes = 0;

void* worker(void*);


void launch(void* data)
{
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





void* worker(void* data)
{
    sqlite3* db = data;

    printf("worker lancer\n");

    while(1){
        Commande* commande = accessCommand();
        printf("Traitement Commande %d\n",commande->type);
        if(commande->type == AJOUT_ELECTEUR){
            if(handlerajoutelecteur(db,commande) == 0){
                printf("Electeur ajouter\n");
            }
            else{
                printf("Error lors de l'ajout");
            }
        }
        else if(commande->type == SUPPRIME_ELECTEUR){
            if(handlersupprimeElecteur(db,commande) == 0){
                printf("Electeur supprimer\n");
            }
            else{
                printf("Error lors de la suppression");
            }
        }
        else if(commande->type == EST_PRESENT){
            if(handlerestpresent(db,commande) == 0){
                printf("Electeur est present\n");
            }
            else{
                printf("Error lors du check est present");
            }
        }
        else{
            printf("Commande inconnu abandon du traitement.");
        }
        free(commande);
    }
}