#include "../../common/include/bd.h"
#include "../../common/include/messages.h"
#include "string.h"

int supprimeElecteur(sqlite3 *db, Commande* cmd) {
    char id=cmd->commande.supprimeElecteur.identifiant;
    if(electeurExists(db, id, strlen(id)+1)){
        deleteElecteur(db, id, strlen(id)+1);
        return 0;
    } else {
        return -1;
    }
}