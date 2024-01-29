
#include "handler.h"

int handlerSupprimeElection(sqlite3 *db, Commande* cmd) {
    char id=cmd->commande.supprimeElection.identifiant;
    if(Election_getIdFromNumeroID(db, id, strlen(id)+1)){
        deleteElection(db, id, strlen(id)+1);
        printf("Election supprimee\n");
        return 0;
    } else {
        return -1;
    }
}