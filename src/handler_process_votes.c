
#include "../common/include/handler.h"
#include <stdio.h>
#include "../common/include/messages.h"
#include "../common/include/bd.h"

// Fonction pour mettre a jour une election
CodeErreur handlerprocederauxresultatsvote(sqlite3 *db, Commande *cmd,mpz_t g,mpz_t lambda, mpz_t mu, mpz_t n,char* result) {
    char* id = cmd->commande.procederAuxResultatsVotes.identifiantElection;
    int veritableId = Election_getIdFromNumeroID(db, id, strlen(id));
    if (veritableId > 0)
    {
        int option1;
        int option0;
        int total_vote;
        Election_procederAuxResultatsVotes(db, veritableId, &option0, &option1, &total_vote, g, lambda, mu, n);
        sprintf(result,"Resultat de l'election: \n Nombre de votant %d\n\t\tOui : %d (%.2f%%)\n\t\tNon : %d (%.2f%%)\n", total_vote, option0, (total_vote != 0) ? (double)option0 * 100. / total_vote : 0.0, option1, (total_vote != 0) ? (double)option1 * 100. / total_vote : 0.0);
        printf("Resultat de l'election process avec succes\n");
        return REUSSITE;
    }
    return ElECTION_PAS_PRESENTE;
}
