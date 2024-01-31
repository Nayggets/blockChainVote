
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

CodeErreur handlerReadElection(sqlite3 *db, Commande* cmd,char* tab) {
    char* id=cmd->commande.readElection.identifiant;
    int intId = Election_getIdFromNumeroID(db, id, strlen(id)+1);
    if(intId != -1){
        
        readElection(db, intId, tab);
        return REUSSITE;
    } else {
        return ElECTION_PAS_PRESENTE;
    }
}