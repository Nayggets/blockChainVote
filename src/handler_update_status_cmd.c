#include "../common/include/handler.h"

int handlerUpdateStatus(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.updateStatus.identifiant;
    int realId = Election_getIdFromNumeroID(db, id, strlen(id));
    if(realId=!-1){
        updateStatus(db, realId, "canceled");
        printf("Vote annulé\n");
        return REUSSITE;
    } else {
        return ElECTION_PAS_PRESENTE;
    }
}