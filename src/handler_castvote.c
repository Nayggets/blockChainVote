
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour ajouter un électeur
int handlercastvote(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.castVote.identifiant;
    char* idElection = cmd->commande.castVote.idElection;
    if (electeurExists(db, id, strlen(id) + 1) == 0 && electionExists(db, idElection, strlen(idElection) + 1) == 0)
    {
        Election_castVote(db, id, idElection, cmd->commande.castVote.ballot, cmd->commande.castVote.ballotSize, cmd->commande.castVote.hashValidation);
        return 0;
    }
    return -1;
}