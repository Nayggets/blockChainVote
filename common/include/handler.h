#ifndef __HANDLER_H
#define __HANDLER_H
#include <stdio.h>
#include <string.h>

#include "./bd.h"
#include "./messages.h"
#include <glib.h>



int handlerajoutelecteur(sqlite3 *db, Commande *cmd);

int handlerestpresent(sqlite3 *db,Commande* cmd);

int handlersupprimeElecteur(sqlite3 *db, Commande* cmd);

int handlercastvote(sqlite3 *db, Commande *cmd);

int handlerupdateelecteur(sqlite3 *db, Commande* cmd);

int handlerReadElecteur(sqlite3 *db, Commande* cmd); 

int handlerAjoutelection(sqlite3 *db, Commande *cmd);

int handlerSupprimeElection(sqlite3 *db, Commande *cmd);

int handlerUpdateElection(sqlite3 *db, Commande *cmd);

int handlerReadElection(sqlite3 *db, Commande *cmd);


#endif