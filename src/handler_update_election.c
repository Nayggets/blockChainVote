
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour une election
CodeErreur handlermiseajourelection(sqlite3 *db, Commande *cmd) {
    char* id = cmd->commande.updateElection.identifiant;
    int veritableId = Election_getIdFromNumeroID(db, id, strlen(id)+1);
    if (veritableId > 0)
    {
        updateElection(db, veritableId, cmd->commande.updateElection.question);
        printf("Election mise a jour");
        return REUSSITE;
    }
    return ElECTION_PAS_PRESENTE;
}
