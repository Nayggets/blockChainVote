
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour un électeur
CodeErreur handlerupdateelecteur(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.updateElecteur.identifiant;
    if (electeurExists(db, id, strlen(id)) == 0)
    {
        
        updateElecteur(db, id, strlen(id), cmd->commande.updateElecteur.newId,strlen(cmd->commande.updateElecteur.newId)+1);
        printf("Electeur mis a jour");
        return REUSSITE;
    }
    return ELECTEUR_PAS_PRESENT;
}
