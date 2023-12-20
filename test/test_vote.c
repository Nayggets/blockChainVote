#include "../common/include/votechain.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define V if (verbose)
#define ID_SIZE 10
#define BALLOT_SIZE 1

int main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;

    // Définition des options
    gboolean verbose = FALSE;
    gchar *file_path = NULL;
    GOptionEntry entries[] = {
        {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Mode verbeux", NULL},
        {"file", 'f', 0, G_OPTION_ARG_STRING, &file_path, "Chemin du fichier", "FILE"},
        {NULL}};

    // Création d'un contexte d'option
    context = g_option_context_new("- test pour la création de la base de donnée");
    g_option_context_add_main_entries(context, entries, NULL);

    // Analyse des options de ligne de commande
    if (!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("Erreur lors de l'analyse des options: %s\n", error->message);
        g_error_free(error);
        g_option_context_free(context);
        return 1;
    }

    // Affichage des résultats
    if (verbose)
    {
        g_print("Mode verbeux activé\n");
    }
    if (file_path != NULL)
    {
        g_print("Chemin du fichier: %s\n", file_path);
        int needInit = database_exists(file_path);
        struct stat buffer;
        g_assert(needInit == -1);
        sqlite3 *db = database_open(file_path);
        g_assert_nonnull(db);
        if (needInit == -1)
        {
            g_print("initilisation de la base de données");
            database_init(db);
        }

        // creation d une electionelection_LocalId
        char idElection[ID_SIZE] = "EXXXXXXXXX";
        const char *question = "This is a binary question";
        createElection(db, idElection, ID_SIZE, question, "01/01/2023", "02/01/2023", "active");
        int election_LocalId = Election_getIdFromNumeroID(db, idElection, ID_SIZE);
        g_assert(election_LocalId == 1);

        // creation des electeurs
        char idElecteur1[ID_SIZE] = "PXXXXXXXX1";
        char idElecteur2[ID_SIZE] = "PXXXXXXXX2";
        char idElecteur3[ID_SIZE] = "PXXXXXXXX3";

        createElecteur(db, idElecteur1, ID_SIZE);
        createElecteur(db, idElecteur2, ID_SIZE);
        createElecteur(db, idElecteur3, ID_SIZE);
        int electeur_n01 = getIdFromNumeroID(db, idElecteur1, ID_SIZE);
        int electeur_n02 = getIdFromNumeroID(db, idElecteur2, ID_SIZE);
        int electeur_n03 = getIdFromNumeroID(db, idElecteur3, ID_SIZE);
        // la phase de vote

        // la phase de vote
        char ballot1[BALLOT_SIZE] = {0x01};
        Election_castVote(db, electeur_n01, election_LocalId, ballot1, BALLOT_SIZE, "TODO");

        char ballot2[BALLOT_SIZE] = {0x00};
        Election_castVote(db, electeur_n02, election_LocalId, ballot2, BALLOT_SIZE, "TODO");

        char ballot3[BALLOT_SIZE] = {0x00};
        Election_castVote(db, electeur_n03, election_LocalId, ballot3, BALLOT_SIZE, "TODO");

        // le résultat de l'election
        int no = 0;
        int yes = 0;
        int total = 0;

        Election_processVotes(db, election_LocalId, &no, &yes, &total);

        printf("Résultat de l'élection: \n A la question '%s'\n Nombre de votant %d\n\t\tOui : %d (%.2f%%)\n\t\tNon : %d (%.2f%%)\n", question, total, yes, (total != 0) ? (double)yes * 100. / total : 0.0, no, (total != 0) ? (double)no * 100. / total : 0.0);
    }
    else
    {
        g_print("Aucun chemin de fichier fourni\n");
    }

    // Libération de la mémoire
    g_free(file_path);
    g_option_context_free(context);
    return 0;
}
