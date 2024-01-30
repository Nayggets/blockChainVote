
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour ajouter un électeur
int handlercastvote(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.castVote.identifiant;
    char* idElection = cmd->commande.castVote.idElection;
    int intId = getIdFromNumeroID(db,cmd->commande.castVote.identifiant,  strlen(cmd->commande.castVote.identifiant)+1);
    int intIdElection = Election_getIdFromNumeroID(db, idElection, strlen(idElection)+1);
    if (intId != -1 && intIdElection != -1)
    {
        Election_castVote(db, intId, intIdElection, &cmd->commande.castVote.ballot, cmd->commande.castVote.ballotSize, NULL);
        return 0;
    }
    return -1;
}