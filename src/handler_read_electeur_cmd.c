
#include "handler.h"

int handlerReadElecteur(sqlite3 *db, Commande* cmd) {
    char id=cmd->commande.readElecteur.identifiant;
    if(electeurExists(db, id, strlen(id)+1)){
        readElecteur(db, id, strlen(id)+1);
        return 0;
    } else {
        return -1;
    }
}