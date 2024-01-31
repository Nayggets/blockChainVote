#include "../common/include/handler.h"

CodeErreur handlersupprimeelecteur(sqlite3 *db, Commande* cmd) {
    char* id=cmd->commande.supprimeElecteur.identifiant;
    if(electeurExists(db, id, strlen(id)+1)){
        deleteElecteur(db, id, strlen(id)+1);
        printf("Electeur supprimer\n");
        return REUSSITE;
    } else {
        return ELECTEUR_PAS_PRESENT;
    }
}