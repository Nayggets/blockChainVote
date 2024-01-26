#include <stdio.h>
#include <string.h>

#include "./bd.h"
#include "./messages.h"


int handlerestpresent(sqlite3 *db,Commande* cmd);

int handlerajoutelecteur(sqlite3 *db, Commande *cmd);

int handlersupprimeElecteur(sqlite3 *db, Commande* cmd);