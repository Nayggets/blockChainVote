
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour une election
int handlerUpdateElection(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.updateElection.identifiant;
    if (Election_getIdFromNumeroID(db, id, strlen(id) + 1) == 0)
    {
        updateElection(db, id, strlen(id)+1, cmd->commande.updateElection.question);
        printf("Election mise a jour");
        return 0;
    }
    return -1;
}
