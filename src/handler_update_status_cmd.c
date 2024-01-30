#include "../common/include/handler.h"

int handlerUpdateStatus(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.updateStatus.identifiant;
    int realId = Election_getIdFromNumeroID(db, id, strlen(id));
    if(id=!-1){
        updateStatus(db, realId, "canceled");
        printf("Vote annulé\n");
        return 0;
    } else {
        return -1;
    }
}