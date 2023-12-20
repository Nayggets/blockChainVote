#include <stdio.h>
#include "messages.h"
#include "bd.h"

// Fonction pour ajouter un électeur
int handlerajoutelecteur(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.ajoutElecteur.identifiant;
    if (electeurExists(db, id, strlen(id) + 1) == 0)
    {
        createElecteur(db, id, strlen(id)+1);
        return 0;
    }
    return -1;
}
