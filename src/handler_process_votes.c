
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour une election
CodeErreur handlerProcessVote(sqlite3 *db, Commande *cmd,mpz_t g,mpz_t lambda, mpz_t mu, mpz_t n,char* result) {
    char* id = cmd->commande.processVotes.identifiantElection;
    int realid = Election_getIdFromNumeroID(db, id, strlen(id)+1);
    if (realid > 0)
    {
        int option1;
        int option0;
        int total_vote;
        Election_processVotes(db, realid, &option0, &option1, &total_vote, g, lambda, mu, n);
        sprintf(result,"Résultat de l'élection: \n Nombre de votant %d\n\t\tOui : %d (%.2f%%)\n\t\tNon : %d (%.2f%%)\n", total_vote, option0, (total_vote != 0) ? (double)option0 * 100. / total_vote : 0.0, option1, (total_vote != 0) ? (double)option1 * 100. / total_vote : 0.0);
        printf("Resultat de l'election process avec succès\n");
        return REUSSITE;
    }
    return ElECTION_PAS_PRESENTE;
}
