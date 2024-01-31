
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

CodeErreur handlerReadElecteur(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.readElecteur.identifiant;

    if(electeurExists(db, id, strlen(id)+1)){
        readElecteur(db, id, strlen(id)+1);
        return REUSSITE;
    } else {
        return ELECTEUR_PAS_PRESENT;
    }
}