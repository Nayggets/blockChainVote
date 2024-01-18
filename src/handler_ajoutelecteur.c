
#include "handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour ajouter un électeur
int handlerajoutelecteur(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.ajoutElecteur.identifiant;
    if (electeurExists(db, id, strlen(id) + 1) == 0)
    {
        printf("Electeur creer\n");
        createElecteur(db, id, strlen(id)+1);
        return 0;
    }
    return -1;
}
