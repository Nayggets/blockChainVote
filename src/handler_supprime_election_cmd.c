
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

CodeErreur handlerSupprimeElection(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.supprimeElection.identifiant;
    int realid = Election_getIdFromNumeroID(db, id, strlen(id)+1);

    if(realid > 0){
        deleteElection(db, realid);
        printf("Election supprimee\n");
        return REUSSITE;
    } else {
        return  ElECTION_PAS_PRESENTE;
    }
}