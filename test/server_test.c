#include "../common/include/serveur_vote.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
int main()
{

    pthread_t pthread;
    int thread = 4;
    launch(&thread);
    printf("Server launch\n");
    Commande commande;
    commande.type = EST_PRESENT;
    EstPresentCmd estpresent;
    strcpy(estpresent.identifiant,"le zizon a evan\0");
    memcpy(&commande.commande,&estpresent,sizeof(EstPresentCmd));
    printf("Commande 1 creer\n");
    Commande commande2;
    commande2.type = SUPPRIME_ELECTEUR;
    SupprimeElecteurCmd supprimeelecteurcmd;
    strcpy(supprimeelecteurcmd.identifiant,"le zizon a evan\0");
    memcpy(&commande2.commande,&supprimeelecteurcmd,sizeof(SupprimeElecteurCmd));
    printf("Commande 2 creer\n");

    Commande commande3;
    commande3.type = AJOUT_ELECTEUR;
    AjoutElecteurCmd ajoutelecteurcmd;
    strcpy(ajoutelecteurcmd.identifiant,"le zizon a evan\0");
    memcpy(&commande3.commande,&ajoutelecteurcmd,sizeof(AjoutElecteurCmd));
    printf("commande %s\ntype %d\n",commande.commande.estPresent.identifiant,commande.type);
    printf("commande %s\ntype %d\n",commande2.commande.estPresent.identifiant,commande2.type);
    printf("commande %s\ntype %d\n",commande3.commande.estPresent.identifiant,commande3.type);

    printf("Commande 3 creer\n");
    fillCommand(commande3);
    fillCommand(commande);
    fillCommand(commande2);
    sleep(20);
}