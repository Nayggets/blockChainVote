#ifndef __HANDLER_H
#define __HANDLER_H
#include <stdio.h>
#include <string.h>

#include "./bd.h"
#include "./messages.h"
#include <glib.h>



CodeErreur handlerajoutelecteur(sqlite3 *db, Commande *cmd);

CodeErreur handlerestpresent(sqlite3 *db,Commande* cmd);

CodeErreur handlersupprimeElecteur(sqlite3 *db, Commande* cmd);

CodeErreur handlercastvote(sqlite3 *db, Commande *cmd);

CodeErreur handlerupdateelecteur(sqlite3 *db, Commande* cmd);

CodeErreur handlerReadElecteur(sqlite3 *db, Commande* cmd); 

CodeErreur handlerAjoutelection(sqlite3 *db, Commande *cmd);

CodeErreur handlerSupprimeElection(sqlite3 *db, Commande *cmd);

<<<<<<< HEAD
CodeErreur handlerUpdateElection(sqlite3 *db, Commande *cmd);

CodeErreur handlerReadElection(sqlite3 *db, Commande *cmd);

int handlercastvote(sqlite3 *db, Commande *cmd);

int handlerUpdateStatus(sqlite3 *db, Commande *cmd);

=======
int handlercastvote(sqlite3 *db, Commande *cmd);

>>>>>>> 3f75a2aaf09d93163cd3656a6ebd3a7cdd191703
#endif
