
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"


#define BALLOT_SIZE 8 // a changer si on monte la taille des bigint
// Fonction pour ajouter un électeur
CodeErreur handlercastvote(sqlite3 *db, Commande *cmd,mpz_t n,mpz_t g) {
    char *id = cmd->commande.castVote.identifiant;
    char* idElection = cmd->commande.castVote.idElection;
    mpz_t m,c;
    mpz_inits(m,c,NULL);
    mpz_set_ui(m, cmd->commande.castVote.ballot);
    encrypt(c,m,n,g);
    char ballot[BALLOT_SIZE];
    mpz_get_str(ballot,10,c);
    int intId = getIdFromNumeroID(db,cmd->commande.castVote.identifiant,  strlen(cmd->commande.castVote.identifiant)+1);
    int intIdElection = Election_getIdFromNumeroID(db, idElection, strlen(idElection)+1);
    int aDejaVote = adejavote(db,intId,intIdElection);
    int voteFermer = verifiesiclosed(db,intId);
    int voteCancel = verifieSiCanceled(db, intId);

    if (intId != -1 && intIdElection != -1 && aDejaVote != 1 && voteFermer == 0 && voteCancel == 0)
    {
        Election_castVote(db, intId, intIdElection, ballot, BALLOT_SIZE, NULL);
        return REUSSITE;
    }
    if (intId == -1)
    {
        return ELECTEUR_PAS_PRESENT;
    }
    if (intIdElection == -1)
    {
        return ElECTION_PAS_PRESENTE;
    }
    return ElECTION_PAS_PRESENTE;
}