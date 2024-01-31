
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour un électeur
CodeErreur handlermiseajourelecteur(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.updateElecteur.identifiant;
    char *nouveauId = cmd->commande.updateElecteur.nouveauId;
    if (electeurExists(db, id, strlen(id)+1) != 0)
    {
        
        updateElecteur(db, id, strlen(id)+1, nouveauId,strlen(nouveauId)+1);
        printf("Electeur mis a jour");
        return REUSSITE;
    }
    return ELECTEUR_PAS_PRESENT;
}
