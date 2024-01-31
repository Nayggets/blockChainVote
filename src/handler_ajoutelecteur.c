
#include "../common/include/handler.h"

// Fonction pour ajouter un électeur
CodeErreur handlerajoutelecteur(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.ajoutElecteur.identifiant;
    if (electeurExists(db, id, strlen(id)+1) == 0)
    {
        createElecteur(db, id, strlen(id)+1);
        printf("Electeur creee");
        return REUSSITE;
    }
    return ELECTEUR_PAS_PRESENT;
}
