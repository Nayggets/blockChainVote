<<<<<<< HEAD:src/handler_ajoutelecteur.c

#include "handler.h"
=======
#include <stdio.h>
#include "../../common/include/messages.h"
#include "../../common/include/bd.h"
>>>>>>> ec704c9eda8bf0f4a92eb2cc0a1f01cab3e63163:src/handler/handler_ajoutelecteur.c

// Fonction pour ajouter un électeur
int handlerajoutelecteur(sqlite3 *db, Commande *cmd) {
    char *id = cmd->commande.ajoutElecteur.identifiant;
    if (electeurExists(db, id, strlen(id) + 1) == 0)
    {
<<<<<<< HEAD:src/handler_ajoutelecteur.c
        createElecteur(db, id, strlen(id)+1);
        printf("Electeur creer");
=======
        createElecteur(db, id, strlen(id) + 1);
>>>>>>> ec704c9eda8bf0f4a92eb2cc0a1f01cab3e63163:src/handler/handler_ajoutelecteur.c
        return 0;
    }
    return -1;
}
