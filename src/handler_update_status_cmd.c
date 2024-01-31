#include "../common/include/handler.h"

int handlermiseajourstatus(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.miseAJourStatus.identifiant;
    int veritableId = Election_getIdFromNumeroID(db, id, strlen(id));
    if(veritableId=!-1){
        miseAJourStatus(db, veritableId, "canceled");
        printf("Vote annulé\n");
        return REUSSITE;
    } else {
        return ElECTION_PAS_PRESENTE;
    }
}