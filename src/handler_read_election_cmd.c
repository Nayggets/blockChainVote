
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

int handlerReadElection(sqlite3 *db, Commande* cmd) {
    char *id=cmd->commande.readElection.identifiant;
    if(Election_getIdFromNumeroID(db, id, strlen(id)+1)){
        readElection(db, id, strlen(id)+1);
        return 0;
    } else {
        return -1;
    }
}