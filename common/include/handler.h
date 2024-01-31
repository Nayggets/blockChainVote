#ifndef __HANDLER_H
#define __HANDLER_H
#include <stdio.h>
#include <string.h>

#include "./bd.h"
#include "./messages.h"
#include <glib.h>



CodeErreur handlerajoutelecteur(sqlite3 *db, Commande *cmd);

CodeErreur handlerestpresent(sqlite3 *db,Commande* cmd);

CodeErreur handlersupprimeelecteur(sqlite3 *db, Commande* cmd);

CodeErreur handlercastvote(sqlite3 *db, Commande *cmd,mpz_t n,mpz_t g);

CodeErreur handlermiseajourelecteur(sqlite3 *db, Commande* cmd);

CodeErreur handlerreadelecteur(sqlite3 *db, Commande* cmd); 

CodeErreur handlerajouteelection(sqlite3 *db, Commande *cmd);

CodeErreur handlersupprimeelection(sqlite3 *db, Commande *cmd);

CodeErreur handlermiseajourelection(sqlite3 *db, Commande *cmd);

CodeErreur handlerreadelection(sqlite3 *db, Commande *cmd);

CodeErreur handlerprocederauxresultatsvote(sqlite3 *db, Commande *cmd,mpz_t g,mpz_t lambda, mpz_t mu, mpz_t n,char* result);


CodeErreur handlermiseajourstatus(sqlite3 *db, Commande *cmd);

#endif