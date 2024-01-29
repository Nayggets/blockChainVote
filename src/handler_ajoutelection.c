
#include "handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour ajouter une election
int handlerAjoutelection(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.ajoutElection.identifiant;
    if (Election_getIdFromNumeroID(db, id, strlen(id) + 1) == -1)
    {
        createElection(db, id, strlen(id)+1, cmd->commande.ajoutElection.question, cmd->commande.ajoutElection.dateDebut, cmd->commande.ajoutElection.dateFin, cmd->commande.ajoutElection.status);
        printf("Election creee");
        return 0;
    }
    return -1;
}
