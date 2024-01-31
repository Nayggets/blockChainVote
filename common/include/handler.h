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

CodeErreur handlercastvote(sqlite3 *db, Commande *cmd,mpz_t n,mpz_t g);

CodeErreur handlerupdateelecteur(sqlite3 *db, Commande* cmd);

CodeErreur handlerReadElecteur(sqlite3 *db, Commande* cmd); 

CodeErreur handlerAjoutelection(sqlite3 *db, Commande *cmd);

CodeErreur handlerSupprimeElection(sqlite3 *db, Commande *cmd);

CodeErreur handlerUpdateElection(sqlite3 *db, Commande *cmd);

CodeErreur handlerReadElection(sqlite3 *db, Commande* cmd,char* tab);

CodeErreur handlerProcessVote(sqlite3 *db, Commande *cmd,mpz_t g,mpz_t lambda, mpz_t mu, mpz_t n,char* result);


CodeErreur handlerUpdateStatus(sqlite3 *db, Commande *cmd);

#endif