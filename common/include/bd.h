#ifndef BD__
#define BD__

#include <sqlite3.h>

sqlite3 *database_open(const char *path);
int database_close(sqlite3 *db);
int database_init(sqlite3 *db);
int database_exists(const char *path);

// usecases electeur
void createElecteur(sqlite3 *db, const char *numeroID, int size);
int electeurExists(sqlite3 *db, const char *numeroID, int size);
void deleteElecteur(sqlite3 *db, const char *numeroID, int size);
void updateElecteur(sqlite3 *db, const char *oldNumeroID, int size1, const char *newNumeroID, int size2);
void readElecteur(sqlite3 *db, const char *numeroID, int size);
int getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);

// usecases election
void createElection(sqlite3 *db, const char *identifiant, int sizeId, const char *question, const char *dateDebut, const char *dateFin, const char *status);
void updateElection(sqlite3 *db, int id, const char *question);
int Election_getIdFromNumeroID(sqlite3 *db, const char *numeroID, int size);

void Election_castVote(sqlite3 *db, int idVotant, int idElection, const void *ballot, int ballotSize, const char *hashValidation);
// int Election_resultat(sqlite3 *db, int localid, int *numberOption0, int *numberOption1, int *totalVotes);
void Election_processVotes(sqlite3 *db, int electionId, int *p_option0, int *p_option1, int *p_totalvotes);
#endif