
#include "handler.h"

int handlersupprimeElecteur(sqlite3 *db, Commande* cmd) {
    char id=cmd->commande.supprimeElecteur.identifiant;
    if(electeurExists(db, id, strlen(id)+1)){
        deleteElecteur(db, id, strlen(id)+1);
        printf("Electeur supprimer\n");
        return 0;
    } else {
        return -1;
    }
}