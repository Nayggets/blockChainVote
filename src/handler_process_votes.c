
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour une election
CodeErreur handlerUpdateElection(sqlite3 *db, Commande *cmd) {
    char* id = cmd->commande.processVotes.identifiantElection;
    int realid = Election_getIdFromNumeroID(db, id, strlen(id));
    if (realid > 0)
    {
        Election_processVotes(db, realid, cmd->commande.processVotes.option0, cmd->commande.processVotes.option1, cmd->commande.processVotes.totalVotes, cmd->commande.processVotes.g, cmd->commande.processVotes.lambda, cmd->commande.processVotes.mu, cmd->commande.processVotes.n);
        printf("Election mise a jour");
        return REUSSITE;
    }
    return ElECTION_PAS_PRESENTE;
}
