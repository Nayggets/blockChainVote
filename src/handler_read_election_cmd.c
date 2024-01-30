
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

CodeErreur handlerReadElection(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.readElection.identifiant;
    if(Election_getIdFromNumeroID(db, id, strlen(id))){
        readElection(db, id, strlen(id));
        return REUSSITE;
    } else {
        return ElECTION_PAS_PRESENTE;
    }
}