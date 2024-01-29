
#include "handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour un électeur
int handlerupdateelecteur(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.updateElecteur.identifiant;
    if (electeurExists(db, id, strlen(id) + 1) == 0)
    {
        newId= cmd->commande.updateElecteur.newId;
        updateElecteur(db, id, strlen(id)+1, newId,strlen(newId)+1);
        printf("Electeur mis a jour");
        return 0;
    }
    return -1;
}
