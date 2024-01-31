
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

CodeErreur handlerreadelecteur(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.readElecteur.identifiant;

    if(electeurExists(db, id, strlen(id))){
        readElecteur(db, id, strlen(id));
        return REUSSITE;
    } else {
        return ELECTEUR_PAS_PRESENT;
    }
}