#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

int date_superior(char* dateDebut,char* dateFin);

// Fonction pour ajouter une election
CodeErreur handlerAjoutelection(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.ajoutElection.identifiant;
    if (Election_getIdFromNumeroID(db, id, strlen(id)+1) == -1)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char date[20];
        sprintf(date, "%2d/%02d/%4d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year+1900);
        printf("date : %s\ndatedebut : %s",date,cmd->commande.ajoutElection.dateDebut);
        if(date_superior(date,cmd->commande.ajoutElection.dateDebut) == -1){
            createElection(db, id, strlen(id)+1, cmd->commande.ajoutElection.question, cmd->commande.ajoutElection.dateDebut, cmd->commande.ajoutElection.dateFin, "closed");
        }
        else{
            createElection(db, id, strlen(id)+1, cmd->commande.ajoutElection.question, cmd->commande.ajoutElection.dateDebut, cmd->commande.ajoutElection.dateFin, "active");
        }
        printf("Election creee");
        return REUSSITE;
    }
    return ElECTION_EXISTE_DEJA;
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
        return -1;
    }
    else if(year_number_fin > year_number_debut){
        return 0;
    }

    if(month_number_fin < month_number_debut){
        return -1;
    }
    else if(month_number_fin > month_number_debut){
        return 0;
    }

    if(day_number_fin < day_number_debut){
        return -1;
    }
    else if(day_number_fin > day_number_debut){
        return 0;
    }

    return 0;
}