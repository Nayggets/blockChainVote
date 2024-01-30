
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour une election
CodeErreur handlerUpdateElection(sqlite3 *db, Commande *cmd) {
    char* id = cmd->commande.updateElection.identifiant;
    int realid = Election_getIdFromNumeroID(db, id, strlen(id));
    if (realid > 0)
    {
        updateElection(db, realid, cmd->commande.updateElection.question);
        printf("Election mise a jour");
        return REUSSITE;
    }
    return ElECTION_PAS_PRESENTE;
}
